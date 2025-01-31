// Copyright 2017 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_DISPLAY_MOJO_DISPLAY_SNAPSHOT_MOJO_STRUCT_TRAITS_H_
#define UI_DISPLAY_MOJO_DISPLAY_SNAPSHOT_MOJO_STRUCT_TRAITS_H_

#include "ui/display/display_snapshot_mojo.h"
#include "ui/display/mojo/display_snapshot_mojo.mojom.h"
#include "ui/display/types/display_mode.h"

namespace mojo {

template <>
struct StructTraits<display::mojom::DisplaySnapshotMojoDataView,
                    std::unique_ptr<display::DisplaySnapshotMojo>> {
  static int64_t display_id(
      const std::unique_ptr<display::DisplaySnapshotMojo>& snapshot) {
    return snapshot->display_id();
  }

  static const gfx::Point& origin(
      const std::unique_ptr<display::DisplaySnapshotMojo>& snapshot) {
    return snapshot->origin();
  }

  static const gfx::Size& physical_size(
      const std::unique_ptr<display::DisplaySnapshotMojo>& snapshot) {
    return snapshot->physical_size();
  }

  static display::DisplayConnectionType type(
      const std::unique_ptr<display::DisplaySnapshotMojo>& snapshot) {
    return snapshot->type();
  }

  static bool is_aspect_preserving_scaling(
      const std::unique_ptr<display::DisplaySnapshotMojo>& snapshot) {
    return snapshot->is_aspect_preserving_scaling();
  }

  static bool has_overscan(
      const std::unique_ptr<display::DisplaySnapshotMojo>& snapshot) {
    return snapshot->has_overscan();
  }

  static bool has_color_correction_matrix(
      const std::unique_ptr<display::DisplaySnapshotMojo>& snapshot) {
    return snapshot->has_color_correction_matrix();
  }

  static std::string display_name(
      const std::unique_ptr<display::DisplaySnapshotMojo>& snapshot) {
    return snapshot->display_name();
  }

  static const base::FilePath& sys_path(
      const std::unique_ptr<display::DisplaySnapshotMojo>& snapshot) {
    return snapshot->sys_path();
  }

  static std::vector<uint8_t> edid(
      const std::unique_ptr<display::DisplaySnapshotMojo>& snapshot) {
    return snapshot->edid();
  }

  static std::vector<std::unique_ptr<display::DisplayMode>> modes(
      const std::unique_ptr<display::DisplaySnapshotMojo>& snapshot);

  static uint64_t current_mode_index(
      const std::unique_ptr<display::DisplaySnapshotMojo>& snapshot);

  static bool has_current_mode(
      const std::unique_ptr<display::DisplaySnapshotMojo>& snapshot) {
    return snapshot->current_mode() != nullptr;
  }

  static uint64_t native_mode_index(
      const std::unique_ptr<display::DisplaySnapshotMojo>& snapshot);

  static bool has_native_mode(
      const std::unique_ptr<display::DisplaySnapshotMojo>& snapshot) {
    return snapshot->native_mode() != nullptr;
  }

  static int64_t product_id(
      const std::unique_ptr<display::DisplaySnapshotMojo>& snapshot) {
    return snapshot->product_id();
  }

  static const gfx::Size& maximum_cursor_size(
      const std::unique_ptr<display::DisplaySnapshotMojo>& snapshot) {
    return snapshot->maximum_cursor_size();
  }

  static bool Read(display::mojom::DisplaySnapshotMojoDataView data,
                   std::unique_ptr<display::DisplaySnapshotMojo>* out);
};

}  // namespace mojo

#endif  // UI_DISPLAY_MOJO_DISPLAY_SNAPSHOT_MOJO_STRUCT_TRAITS_H_
