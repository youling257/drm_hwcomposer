/*
 * Copyright (C) 2015 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef ANDROID_PLATFORM_DRM_GENERIC_H_
#define ANDROID_PLATFORM_DRM_GENERIC_H_

#include "drmresources.h"
#include "platform.h"

#include <hardware/gralloc.h>
#include <map>

namespace android {

class DrmGenericImporter : public Importer {
 public:
  DrmGenericImporter(DrmResources *drm);
  ~DrmGenericImporter() override;

  int Init();

  int ImportBuffer(buffer_handle_t handle, hwc_drm_bo_t *bo) override;
  int ReleaseBuffer(hwc_drm_bo_t *bo) override;
  int ImportHandle(uint32_t gem_handle);
  int ReleaseHandle(uint32_t gem_handle);

  uint32_t ConvertHalFormatToDrm(uint32_t hal_format);
  bool GetYuvPlaneInfo(int num_fds, buffer_handle_t handle, hwc_drm_bo_t *bo);

 private:

  DrmResources *drm_;

  const gralloc_module_t *gralloc_;
  int CloseHandle(uint32_t gem_handle);
  std::map<uint32_t, int> gem_refcount_;
};
}

#endif
