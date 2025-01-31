/*
 * Copyright (C) 2010 Google Inc. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1.  Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 * 2.  Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE INC. AND ITS CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL APPLE INC. OR ITS CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "core/dom/DOMTokenList.h"

#include "bindings/core/v8/ExceptionState.h"
#include "core/dom/ExceptionCode.h"
#include "core/html/parser/HTMLParserIdioms.h"
#include "wtf/text/StringBuilder.h"

namespace blink {

bool DOMTokenList::validateToken(const String& token,
                                 ExceptionState& exceptionState) const {
  if (token.isEmpty()) {
    exceptionState.throwDOMException(SyntaxError,
                                     "The token provided must not be empty.");
    return false;
  }

  if (token.find(isHTMLSpace) != kNotFound) {
    exceptionState.throwDOMException(InvalidCharacterError,
                                     "The token provided ('" + token +
                                         "') contains HTML space characters, "
                                         "which are not valid in tokens.");
    return false;
  }

  return true;
}

bool DOMTokenList::validateTokens(const Vector<String>& tokens,
                                  ExceptionState& exceptionState) const {
  for (const auto& token : tokens) {
    if (!validateToken(token, exceptionState))
      return false;
  }

  return true;
}

// https://dom.spec.whatwg.org/#concept-domtokenlist-validation
bool DOMTokenList::validateTokenValue(const AtomicString&,
                                      ExceptionState& exceptionState) const {
  exceptionState.throwTypeError("DOMTokenList has no supported tokens.");
  return false;
}

bool DOMTokenList::contains(const AtomicString& token,
                            ExceptionState& exceptionState) const {
  if (!validateToken(token, exceptionState))
    return false;
  return containsInternal(token);
}

void DOMTokenList::add(const AtomicString& token,
                       ExceptionState& exceptionState) {
  Vector<String> tokens;
  tokens.push_back(token.getString());
  add(tokens, exceptionState);
}

// Optimally, this should take a Vector<AtomicString> const ref in argument but
// the bindings generator does not handle that.
void DOMTokenList::add(const Vector<String>& tokens,
                       ExceptionState& exceptionState) {
  Vector<String> filteredTokens;
  filteredTokens.reserveCapacity(tokens.size());
  for (const auto& token : tokens) {
    if (!validateToken(token, exceptionState))
      return;
    if (containsInternal(AtomicString(token)))
      continue;
    if (filteredTokens.contains(token))
      continue;
    filteredTokens.push_back(token);
  }

  if (!filteredTokens.isEmpty())
    setValue(addTokens(value(), filteredTokens));
}

void DOMTokenList::remove(const AtomicString& token,
                          ExceptionState& exceptionState) {
  Vector<String> tokens;
  tokens.push_back(token.getString());
  remove(tokens, exceptionState);
}

// Optimally, this should take a Vector<AtomicString> const ref in argument but
// the bindings generator does not handle that.
void DOMTokenList::remove(const Vector<String>& tokens,
                          ExceptionState& exceptionState) {
  if (!validateTokens(tokens, exceptionState))
    return;

  // Check using containsInternal first since it is a lot faster than going
  // through the string character by character.
  bool found = false;
  for (const auto& token : tokens) {
    if (containsInternal(AtomicString(token))) {
      found = true;
      break;
    }
  }

  setValue(found ? removeTokens(value(), tokens) : value());
}

bool DOMTokenList::toggle(const AtomicString& token,
                          ExceptionState& exceptionState) {
  if (!validateToken(token, exceptionState))
    return false;

  if (containsInternal(token)) {
    removeInternal(token);
    return false;
  }
  addInternal(token);
  return true;
}

bool DOMTokenList::toggle(const AtomicString& token,
                          bool force,
                          ExceptionState& exceptionState) {
  if (!validateToken(token, exceptionState))
    return false;

  if (force)
    addInternal(token);
  else
    removeInternal(token);

  return force;
}

bool DOMTokenList::supports(const AtomicString& token,
                            ExceptionState& exceptionState) {
  return validateTokenValue(token, exceptionState);
}

void DOMTokenList::addInternal(const AtomicString& token) {
  if (!containsInternal(token))
    setValue(addToken(value(), token));
}

void DOMTokenList::removeInternal(const AtomicString& token) {
  // Check using contains first since it uses AtomicString comparisons instead
  // of character by character testing.
  if (!containsInternal(token))
    return;
  setValue(removeToken(value(), token));
}

AtomicString DOMTokenList::addToken(const AtomicString& input,
                                    const AtomicString& token) {
  Vector<String> tokens;
  tokens.push_back(token.getString());
  return addTokens(input, tokens);
}

// This returns an AtomicString because it is always passed as argument to
// setValue() and setValue() takes an AtomicString in argument.
AtomicString DOMTokenList::addTokens(const AtomicString& input,
                                     const Vector<String>& tokens) {
  bool needsSpace = false;

  StringBuilder builder;
  if (!input.isEmpty()) {
    builder.append(input);
    needsSpace = !isHTMLSpace<UChar>(input[input.length() - 1]);
  }

  for (const auto& token : tokens) {
    if (needsSpace)
      builder.append(' ');
    builder.append(token);
    needsSpace = true;
  }

  return builder.toAtomicString();
}

AtomicString DOMTokenList::removeToken(const AtomicString& input,
                                       const AtomicString& token) {
  Vector<String> tokens;
  tokens.push_back(token.getString());
  return removeTokens(input, tokens);
}

// This returns an AtomicString because it is always passed as argument to
// setValue() and setValue() takes an AtomicString in argument.
AtomicString DOMTokenList::removeTokens(const AtomicString& input,
                                        const Vector<String>& tokens) {
  // Algorithm defined at
  // http://www.whatwg.org/specs/web-apps/current-work/multipage/common-microsyntaxes.html#remove-a-token-from-a-string
  // New spec is at https://dom.spec.whatwg.org/#remove-a-token-from-a-string

  unsigned inputLength = input.length();
  StringBuilder output;  // 3
  output.reserveCapacity(inputLength);
  unsigned position = 0;  // 4

  // Step 5
  while (position < inputLength) {
    if (isHTMLSpace<UChar>(input[position])) {  // 6
      position++;
      continue;  // 6.3
    }

    // Step 7
    StringBuilder tokenBuilder;
    while (position < inputLength && isNotHTMLSpace<UChar>(input[position]))
      tokenBuilder.append(input[position++]);

    // Step 8
    String token = tokenBuilder.toString();
    if (tokens.contains(token)) {
      // Step 8.1
      while (position < inputLength && isHTMLSpace<UChar>(input[position]))
        ++position;

      // Step 8.2
      size_t j = output.length();
      while (j > 0 && isHTMLSpace<UChar>(output[j - 1]))
        --j;
      output.resize(j);
    } else {
      output.append(token);  // Step 9
    }

    if (position < inputLength && !output.isEmpty())
      output.append(' ');
  }

  size_t j = output.length();
  if (j > 0 && isHTMLSpace<UChar>(output[j - 1]))
    output.resize(j - 1);

  return output.toAtomicString();
}

void DOMTokenList::setValue(const AtomicString& value) {
  bool valueChanged = m_value != value;
  m_value = value;
  if (valueChanged)
    m_tokens.set(value, SpaceSplitString::ShouldNotFoldCase);
  if (m_observer)
    m_observer->valueWasSet();
}

bool DOMTokenList::containsInternal(const AtomicString& token) const {
  return m_tokens.contains(token);
}

const AtomicString DOMTokenList::item(unsigned index) const {
  if (index >= length())
    return AtomicString();
  return m_tokens[index];
}

}  // namespace blink
