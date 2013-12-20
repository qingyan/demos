#define LOG_TAG "Demo"

#include <utils/Log.h>
#include <utils/misc.h>

#include <gui/SurfaceComposerClient.h>
#include <ui/Region.h>
#include <ui/Rect.h>

#include "surfaceflinger.h"

using namespace android;

static sp<SurfaceComposerClient> android_client;
static sp<Surface> android_surface;
static sp<SurfaceControl> android_control;

#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

char *surfaceflinger_lock(void)
{
	Surface::SurfaceInfo info;

	android_surface->lock(&info);

	return (char *) info.bits;
}

void surfaceflinger_unlock(void)
{
	android_surface->unlockAndPost();
}

static int surfaceflinger_format(int depth)
{
	int fmt;

	switch (depth) {
	case 16:
		fmt = PIXEL_FORMAT_RGB_565;
		break;
	case 32:
		fmt = PIXEL_FORMAT_RGBA_8888;
		break;
	default:
		fmt = PIXEL_FORMAT_UNKNOWN;
		break;
	}

	return fmt;
}

void surfaceflinger_fini(void)
{
	android_surface = NULL;
    android_control = NULL;
	android_client = NULL;
}

int surfaceflinger_init(int x, int y, int w, int h, int *stride)
{
	int depth = 32;
	int fmt;

	android_client = new SurfaceComposerClient;
	if (android_client == NULL) {
		LOGE("failed to create client\n");
		return 0;
	}

	fmt = surfaceflinger_format(depth);
	if (fmt == PIXEL_FORMAT_UNKNOWN) {
		LOGE("failed to find a format for depth %d\n", depth);
		return 0;
	}

	android_control = android_client->createSurface(String8("surfaceflinger"), w, h, fmt, 0);
    android_surface = android_control->getSurface();
	if (android_surface == NULL) {
		LOGE("failed to create surface\n");
		return 0;
	}

	android_client->openGlobalTransaction();
	android_control->setPosition(x, y);
	android_control->setLayer(INT_MAX);
	android_client->closeGlobalTransaction();

	if (stride)
		*stride = w * depth / 8;

	return 1;
}
