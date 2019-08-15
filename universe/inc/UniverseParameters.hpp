#ifndef UNIVERSE_PARAMETERS_HPP_
#define UNIVERSE_PARAMETERS_HPP_

#define RAW_INT_MAX 0xfffff

#pragma pack(push, 1)
struct Params {
    int32_t octaveCount;
    int32_t frequency;
    int32_t stepSize;
    int32_t xOffset;
    int32_t yOffset;
    int32_t minValue;
    int32_t zOffset;

    Params() {
        octaveCount = 1;
        frequency = 1;
        stepSize = 1;
        xOffset = 1;
        yOffset = 1;
        minValue = 1;
        zOffset = 1;
        Load();
    }
    void Save() {
        FILE * f = fopen("params.bin", "wb");
        if (f != NULL) {
            int n = fwrite(this, sizeof(Params), 1, f);
            fclose(f);
            if (n != 1) {
                throw;
            }
        } else {
            throw;
        }
    }
    void Load() {
        FILE * f = fopen("params.bin", "rb");
        if (f != NULL) {
            int n = fread(this, sizeof(Params), 1, f);
            fclose(f);
            if (n != 1) {
                throw;
            }
        } else {
            throw;
        }
    }
};
#pragma pack(pop)

template<typename T>
class Parameter {
public:
    T value;

public:
    Parameter() {
        value = 0;
    }
    void Set(T v) {
        value = v;
    }
    void Set(T v, T vmin, T vmax) {
        value = v;
        value_min_ = vmin;
        value_max_ = vmax;
    }
    void SetMinMax(T min, T max) {
        value_min_ = min;
        value_max_ = max;
    }
    bool SetRawValue(int raw_value) {
        T valueSpan = value_max_ - value_min_;
        T tRaw = static_cast<T>(raw_value);
        T tRawMax = static_cast<T>(RAW_INT_MAX);
        T newValue = (valueSpan * tRaw / tRawMax) + value_min_;
        if (value != newValue) {
            value = newValue;
            return true;
        } else {
            return false;
        }
    }

private:
    T value_min_;
    T value_max_;
};

struct UniverseParameters {

    Parameter<int> octaveCount;
    // int octaveCount;
    Parameter<double> frequency;
    Parameter<double> x;
    Parameter<double> y;
    Parameter<double> minValue;
    Parameter<double> stepSize;
    Parameter<double> zIndex;

    UniverseParameters() {
        octaveCount.Set(1, 1, 16);
        frequency.Set(1.648, 0.1, 64.0);
        x.Set(-14.507, -100.0, 100.0);
        y.Set( 13.707, -100.0, 100.0);
        minValue.Set(0.637, 0.0, 1.0);
        stepSize.Set(0.529, 0.1, 1.0);
        zIndex.Set(0.0, -10.0, 10.0);
    }
    void Update() {
        updated_ |= octaveCount.SetRawValue(rawParams.octaveCount);
        updated_ |= frequency.SetRawValue(rawParams.frequency);
        updated_ |= x.SetRawValue(rawParams.xOffset);
        updated_ |= y.SetRawValue(rawParams.yOffset);
        updated_ |= minValue.SetRawValue(rawParams.minValue);
        updated_ |= stepSize.SetRawValue(rawParams.stepSize);
        updated_ |= zIndex.SetRawValue(rawParams.zOffset);
    }
    bool updated_;
    bool CheckUpdate() {
        bool result = false;
        if (updated_) {
            result = updated_;
            updated_ = false;
        }
        return result;
    }
    void Save() {
        rawParams.Save();
    }
    void Load() {
        rawParams.Load();
        Update();
    }

public:
    Params rawParams;
};

#endif // UNIVERSE_PARAMETERS_HPP_
