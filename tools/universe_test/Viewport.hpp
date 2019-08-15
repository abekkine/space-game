#ifndef VIEWPORT_HPP_
#define VIEWPORT_HPP_

#include "ScreenPosition.hpp"
#include "WorldPosition.hpp"

#include <GL/glut.h>

class Viewport {
public:
    Viewport() {
        left_ = -1.0;
        right_ = 1.0;
        bottom_ = -1.0;
        top_ = 1.0;
        near_ = -1.0;
        far_ = 1.0;
        center_.Set(0.0, 0.0);
        pixel_size_ = 0.0015;
    }
    ~Viewport() {}
    void GetCenter(WorldPosition & center) {
        center = center_;
    }
    void GetWorldForCursor(const ScreenPosition & p, WorldPosition & w) {
        w.x = left_ + p.x * (right_ - left_) / window_width_;
        w.y = top_ - p.y * (top_ - bottom_) / window_height_;
    }
    double GetSize() {
        return (right_ - left_);
    }
    double GetPixelSize() {
        return pixel_size_;
    }
    void SetWindowSize(const double & w, const double & h) {
        window_width_ = w;
        window_height_ = h;
        zoom_max_distance_ = FastSqrt(w * w + h * h) >> 1;
        Update();
    }
    void Ortho() {
        glOrtho(left_, right_, bottom_, top_, near_, far_);
    }
    void Update() {
        left_ = center_.x - 0.5 * pixel_size_ * window_width_;
        right_ = center_.x + 0.5 * pixel_size_ * window_width_;
        bottom_ = center_.y - 0.5 * pixel_size_ * window_height_;
        top_ = center_.y + 0.5 * pixel_size_ * window_height_;
    }
    void UpdateCursor(const ScreenPosition & cursor) {

        Pan(cursor);
        Zoom(cursor);
    }
    void PanStart(const ScreenPosition & p) {
        pan_mode_ = true;
        pan_start_x_ = p.x;
        pan_start_y_ = p.y;
        pan_start_center_ = center_;
    }
    void PanStop() {
        pan_mode_ = false;
    }
    void ZoomStart(const ScreenPosition & p) {
        zoom_mode_ = true;
        zoom_start_distance_ = GetZoomDistance(p.x, p.y);
        zoom_start_pixel_size_ = pixel_size_;
    }
    void ZoomStop() {
        zoom_mode_ = false;
    }
    void ZoomInAt(const ScreenPosition & p) {

        ZoomAt(p, 0.99);
    }
    void ZoomOutAt(const ScreenPosition & p) {

        ZoomAt(p, 1.01);
    }

private:
    void Pan(const ScreenPosition & p) {
        if (pan_mode_ == false) {
            return;
        }

        int dx = p.x - pan_start_x_;
        int dy = p.y - pan_start_y_;

        center_.x = pan_start_center_.x - dx * pixel_size_;
        center_.y = pan_start_center_.y + dy * pixel_size_;

        Update();
    }
    void Zoom(const ScreenPosition & p) {
        if (zoom_mode_ == false) {
            return;
        }

        zoom_distance_ = GetZoomDistance(p.x, p.y);

        double zoom_ratio = (float)(zoom_distance_ - zoom_start_distance_)/(float)zoom_max_distance_;
        pixel_size_ = zoom_start_pixel_size_ * (1.0 - zoom_ratio);

        Update();
    }

    void ZoomAt(const ScreenPosition & p, double ratio) {

        double pixel_size_0 = pixel_size_;
        pixel_size_ *= ratio;

        left_ += p.x * (pixel_size_0 - pixel_size_);
        top_ -= p.y * (pixel_size_0 - pixel_size_);
        right_ = left_ + window_width_ * pixel_size_;
        bottom_ = top_ - window_height_ * pixel_size_;

        center_.x = 0.5 * (left_ + right_);
        center_.y = 0.5 * (top_ + bottom_);
    }
    int GetZoomDistance(const int x, const int y) {
        int dx = x - 0.5 * window_width_;
        int dy = y - 0.5 * window_height_;
        return FastSqrt(dx * dx + dy * dy);
    }
    uint32_t FastSqrt(uint32_t value) {
        register uint32_t root, remainder, place;
        root = 0;
        remainder = value;
        place = 0x4000000;

        while (place > remainder) {
            place = place >> 2;
        }

        while (place) {
            if (remainder >= root + place) {
                remainder -= root + place;
                root += place << 1;
            }
            root = root >> 1;
            place = place >> 2;
        }

        return root;
    }

private:
    // World center
    WorldPosition center_;
    // Pixel size
    double pixel_size_;
    // Viewport limits
    double left_;
    double right_;
    double bottom_;
    double top_;
    double near_;
    double far_;
    // Window size for calculations
    double window_width_;
    double window_height_;
    // Pan Mode
    bool pan_mode_;
    int pan_start_x_;
    int pan_start_y_;
    WorldPosition pan_start_center_;
    // Zoom Mode
    bool zoom_mode_;
    int zoom_start_distance_;
    double zoom_start_pixel_size_;
    int zoom_distance_;
    int zoom_max_distance_;
};

#endif // VIEWPORT_HPP_
