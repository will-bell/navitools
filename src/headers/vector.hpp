#pragma once

#include <vector>

class BaseVector {
public:
    BaseVector() {}

    explicit BaseVector(const std::vector<double>& data) {
        _data = data;
    }

    // Read-only access when indexing the data
    const double operator[](int index) const {
        return _data[index];
    }

    size_t size() { return _data.size(); }

    const std::vector<double> data() const { return _data; }

    const std::vector<double>* data_ptr() const { return &_data; }

protected:
    std::vector<double> _data;

    void push_back(double value) {_data.push_back(value);}
};

class Vector2 : public BaseVector {
public:
    using BaseVector::BaseVector;

    Vector2(double x, double y) {
        _data.push_back(x);
        _data.push_back(y);
    }

    double x() const {return _data[0];}

    double y() const {return _data[1];}
};

struct Vector3 : BaseVector {
public:
    Vector3(double x, double y, double z) {
        _data.push_back(x);
        _data.push_back(y);
        _data.push_back(z);
    }

    // Read-only access when indexing the data
    const double operator[](int index) const {
        return _data[index];
    }

    double x() const {return _data[0];}

    double y() const {return _data[1];}

    double z() const {return _data[2];}
};

double magnitude(const Vector2& u);
Vector2 scale(const Vector2& u, double alpha);
Vector2 normalize(const Vector2& u);
Vector2 add(const Vector2& u, const Vector2& v);
Vector2 subtract(const Vector2& u, const Vector2& v);

double magnitude(const Vector3& u);
Vector3 scale(const Vector3& u, double alpha);
Vector3 normalize(const Vector3& u);
Vector3 add(const Vector3& u, const Vector3& v);
Vector3 subtract(const Vector3& u, const Vector3& v);
