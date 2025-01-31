// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef DEVICE_USB_USB_DEVICE_ANDROID_H_
#define DEVICE_USB_USB_DEVICE_ANDROID_H_

#include "base/android/scoped_java_ref.h"
#include "base/memory/weak_ptr.h"
#include "device/usb/usb_device.h"

namespace base {
class SequencedTaskRunner;
}

namespace device {

class UsbServiceAndroid;

class UsbDeviceAndroid : public UsbDevice {
 public:
  static scoped_refptr<UsbDeviceAndroid> Create(
      JNIEnv* env,
      base::WeakPtr<UsbServiceAndroid> service,
      scoped_refptr<base::SequencedTaskRunner> blocking_task_runner,
      const base::android::JavaRef<jobject>& usb_device);

  // UsbDevice:
  void RequestPermission(const ResultCallback& callback) override;
  bool permission_granted() const override;
  void Open(const OpenCallback& callback) override;

  jint device_id() const { return device_id_; }
  void PermissionGranted(bool granted);

 private:
  UsbDeviceAndroid(
      JNIEnv* env,
      base::WeakPtr<UsbServiceAndroid> service,
      uint16_t usb_version,
      uint8_t device_class,
      uint8_t device_subclass,
      uint8_t device_protocol,
      uint16_t vendor_id,
      uint16_t product_id,
      uint16_t device_version,
      const base::string16& manufacturer_string,
      const base::string16& product_string,
      const base::string16& serial_number,
      scoped_refptr<base::SequencedTaskRunner> blocking_task_runner,
      const base::android::JavaRef<jobject>& wrapper);
  ~UsbDeviceAndroid() override;

  void CallRequestPermissionCallbacks(bool granted);
  void OnDeviceOpenedToReadDescriptors(
      scoped_refptr<UsbDeviceHandle> device_handle);
  void OnReadDescriptors(scoped_refptr<UsbDeviceHandle> device_handle,
                         std::unique_ptr<UsbDeviceDescriptor> descriptor);
  void OnReadWebUsbDescriptors(
      scoped_refptr<UsbDeviceHandle> device_handle,
      std::unique_ptr<WebUsbAllowedOrigins> allowed_origins,
      const GURL& landing_page);

  scoped_refptr<base::SequencedTaskRunner> blocking_task_runner_;

  const jint device_id_;
  bool permission_granted_ = false;
  std::list<ResultCallback> request_permission_callbacks_;
  base::WeakPtr<UsbServiceAndroid> service_;

  // Java object org.chromium.device.usb.ChromeUsbDevice.
  base::android::ScopedJavaGlobalRef<jobject> j_object_;
};

}  // namespace device

#endif  // DEVICE_USB_USB_DEVICE_ANDROID_H_
