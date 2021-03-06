/*
 * Copyright (C) 2018 The Android Open Source Project
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

#define LOG_TAG "hwc-platform-drm-minigbm"

#include "platformminigbm.h"
#include "drmdevice.h"
#include "platform.h"

#include <xf86drm.h>
#include <xf86drmMode.h>

#include <hardware/gralloc.h>
#include <log/log.h>

#include "cros_gralloc_handle.h"

#define DRM_FORMAT_YVU420_ANDROID fourcc_code('9', '9', '9', '7')

namespace android {

Importer *Importer::CreateInstance(DrmDevice *drm) {
  DrmMinigbmImporter *importer = new DrmMinigbmImporter(drm);
  if (!importer)
    return NULL;

  int ret = importer->Init();
  if (ret) {
    ALOGE("Failed to initialize the minigbm importer %d", ret);
    delete importer;
    return NULL;
  }
  return importer;
}

int DrmMinigbmImporter::ConvertBoInfo(buffer_handle_t handle,
                                      hwc_drm_bo_t *bo) {
  cros_gralloc_handle *gr_handle = (cros_gralloc_handle *)handle;
  if (!gr_handle)
    return -EINVAL;

  bo->width = gr_handle->width;
  bo->height = gr_handle->height;
  bo->hal_format = gr_handle->droid_format;

  bo->format = gr_handle->format;
  if (bo->format == DRM_FORMAT_YVU420_ANDROID)
    bo->format = DRM_FORMAT_YVU420;

  bo->usage = gr_handle->usage;

  for (size_t i = 0; i < gr_handle->num_planes; i++) {
    bo->modifiers[i] = gr_handle->format_modifier;
    bo->pixel_stride = gr_handle->pixel_stride;
    bo->prime_fds[i] = gr_handle->fds[i];
    bo->pitches[i] = gr_handle->strides[i];
    bo->offsets[i] = gr_handle->offsets[i];
  }

  return 0;
}

}  // namespace android
