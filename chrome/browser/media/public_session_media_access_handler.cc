// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "chrome/browser/media/public_session_media_access_handler.h"

#include <set>
#include <utility>

#include "base/bind.h"
#include "base/bind_helpers.h"
#include "base/memory/ptr_util.h"
#include "chrome/browser/chromeos/extensions/public_session_permission_helper.h"
#include "chrome/browser/profiles/profiles_state.h"
#include "chromeos/login/login_state.h"
#include "content/public/browser/web_contents.h"
#include "extensions/common/extension.h"
#include "extensions/common/permissions/manifest_permission_set.h"
#include "extensions/common/permissions/permission_set.h"
#include "extensions/common/url_pattern_set.h"

PublicSessionMediaAccessHandler::PublicSessionMediaAccessHandler() {}

PublicSessionMediaAccessHandler::~PublicSessionMediaAccessHandler() {}

bool PublicSessionMediaAccessHandler::SupportsStreamType(
    const content::MediaStreamType type,
    const extensions::Extension* extension) {
  return extension_media_access_handler_.SupportsStreamType(type, extension);
}

bool PublicSessionMediaAccessHandler::CheckMediaAccessPermission(
    content::WebContents* web_contents,
    const GURL& security_origin,
    content::MediaStreamType type,
    const extensions::Extension* extension) {
  return extension_media_access_handler_.CheckMediaAccessPermission(
      web_contents, security_origin, type, extension);
}

void PublicSessionMediaAccessHandler::HandleRequest(
    content::WebContents* web_contents,
    const content::MediaStreamRequest& request,
    const content::MediaResponseCallback& callback,
    const extensions::Extension* extension) {
  // This class handles requests for Public Sessions only, outside of them just
  // pass the request through to the original class.
  if (!profiles::IsPublicSession() || !extension->is_platform_app()) {
    return extension_media_access_handler_.HandleRequest(web_contents, request,
                                                         callback, extension);
  }

  // This Unretained is safe because the lifetime of this object is until
  // process exit (living inside a base::Singleton object).
  auto prompt_resolved_callback = base::Bind(
      &PublicSessionMediaAccessHandler::ChainHandleRequest,
      base::Unretained(this), web_contents, request, callback, extension);

  extensions::PermissionIDSet requested_permissions;
  if (request.audio_type == content::MEDIA_DEVICE_AUDIO_CAPTURE)
    requested_permissions.insert(extensions::APIPermission::kAudioCapture);
  if (request.video_type == content::MEDIA_DEVICE_VIDEO_CAPTURE)
    requested_permissions.insert(extensions::APIPermission::kVideoCapture);

  extensions::permission_helper::HandlePermissionRequest(
      *extension, requested_permissions, web_contents, prompt_resolved_callback,
      extensions::permission_helper::PromptFactory());
}

void PublicSessionMediaAccessHandler::ChainHandleRequest(
    content::WebContents* web_contents,
    const content::MediaStreamRequest& request,
    const content::MediaResponseCallback& callback,
    const extensions::Extension* extension,
    const extensions::PermissionIDSet& allowed_permissions) {
  content::MediaStreamRequest request_copy(request);

  // If the user denies audio or video capture, here it gets filtered out from
  // the request before being passed on to the actual implementation.
  if (!allowed_permissions.ContainsID(extensions::APIPermission::kAudioCapture))
    request_copy.audio_type = content::MEDIA_NO_SERVICE;
  if (!allowed_permissions.ContainsID(extensions::APIPermission::kVideoCapture))
    request_copy.video_type = content::MEDIA_NO_SERVICE;

  // Pass the request through to the original class.
  extension_media_access_handler_.HandleRequest(web_contents, request_copy,
                                                callback, extension);
}
