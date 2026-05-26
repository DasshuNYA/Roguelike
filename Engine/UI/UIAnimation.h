// @file UIAnimation.h

#pragma once

namespace Engine
{
class UIAnimation
{
   public:
    void Show();
    void Hide();
    void Update(float deltaTime);

    void SetFadeSpeed(float speed);
    void SetAlpha(float value);

    bool IsVisible() const;
    bool IsFullyHidden() const;

    float GetAlpha() const;

   private:
    bool isVisible = true;

    float alpha = 255.0f;
    float targetAlpha = 255.0f;
    float fadeSpeed = 700.0f;
};
}  // namespace Engine