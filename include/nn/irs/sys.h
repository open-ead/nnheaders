#pragma once

namespace nn::applet {
struct AppletResourceUserId;
}  // namespace nn::applet

namespace nn::irsensor::system {
void SetAppletResourceUserId(const applet::AppletResourceUserId&);
void RegisterAppletResourceUserId(const applet::AppletResourceUserId&, bool);
void UnregisterAppletResourceUserId(const applet::AppletResourceUserId&);
void EnableAppletToGetInput(const applet::AppletResourceUserId&, bool);
}  // namespace nn::irsensor::system
