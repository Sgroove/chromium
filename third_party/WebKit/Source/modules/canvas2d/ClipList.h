// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef ClipList_h
#define ClipList_h

#include "platform/graphics/GraphicsTypes.h"
#include "platform/graphics/paint/PaintCanvas.h"
#include "wtf/Allocator.h"
#include "wtf/Vector.h"

class SkPath;

namespace blink {

class ClipList {
  DISALLOW_NEW();

 public:
  ClipList() {}
  ClipList(const ClipList&);
  ~ClipList() {}

  void clipPath(const SkPath&, AntiAliasingMode, const SkMatrix&);
  void playback(PaintCanvas*) const;
  const SkPath& getCurrentClipPath() const;

 private:
  struct ClipOp {
    SkPath m_path;
    AntiAliasingMode m_antiAliasingMode;

    ClipOp();
    ClipOp(const ClipOp&);
  };

  // Number of clip ops that can be stored in a ClipList without resorting to
  // dynamic allocation
  static const size_t cInlineClipOpCapacity = 4;

  WTF::Vector<ClipOp, cInlineClipOpCapacity> m_clipList;
  SkPath m_currentClipPath;
};

}  // namespace blink

#endif
