/*
 * Copyright (C) 1999-2003 Lars Knoll (knoll@kde.org)
 * Copyright (C) 2004, 2005, 2006, 2007, 2008 Apple Inc. All rights reserved.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public License
 * along with this library; see the file COPYING.LIB.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 */

#ifndef CSSQuadValue_h
#define CSSQuadValue_h

#include "core/CoreExport.h"
#include "core/css/CSSValue.h"
#include "wtf/RefPtr.h"

namespace blink {

class CORE_EXPORT CSSQuadValue : public CSSValue {
 public:
  enum TypeForSerialization { SerializeAsRect, SerializeAsQuad };

  static CSSQuadValue* create(CSSValue* top,
                              CSSValue* right,
                              CSSValue* bottom,
                              CSSValue* left,
                              TypeForSerialization serializationType) {
    return new CSSQuadValue(top, right, bottom, left, serializationType);
  }

  CSSValue* top() const { return m_top.get(); }
  CSSValue* right() const { return m_right.get(); }
  CSSValue* bottom() const { return m_bottom.get(); }
  CSSValue* left() const { return m_left.get(); }

  TypeForSerialization serializationType() { return m_serializationType; }

  String customCSSText() const;

  bool equals(const CSSQuadValue& other) const {
    return dataEquivalent(m_top, other.m_top) &&
           dataEquivalent(m_right, other.m_right) &&
           dataEquivalent(m_left, other.m_left) &&
           dataEquivalent(m_bottom, other.m_bottom);
  }

  DECLARE_TRACE_AFTER_DISPATCH();

 protected:
  CSSQuadValue(CSSValue* top,
               CSSValue* right,
               CSSValue* bottom,
               CSSValue* left,
               TypeForSerialization serializationType)
      : CSSValue(QuadClass),
        m_serializationType(serializationType),
        m_top(top),
        m_right(right),
        m_bottom(bottom),
        m_left(left) {}

 private:
  TypeForSerialization m_serializationType;
  Member<CSSValue> m_top;
  Member<CSSValue> m_right;
  Member<CSSValue> m_bottom;
  Member<CSSValue> m_left;
};

DEFINE_CSS_VALUE_TYPE_CASTS(CSSQuadValue, isQuadValue());

}  // namespace blink

#endif  // CSSQuadValue_h
