#include "Lerp.h"

Vector3 Lerp(const Vector3& v1, const Vector3& v2, float t) {
	Vector3 result = Add(v1, Multiply(t, Subtract(v2, v1)));
	return result;
}

Vector3 Slerp(const Vector3& v1, const Vector3& v2, float t) {
	float s = ((1 - t) * Length(v1)) + (t * Length(v2));
	Vector3 e1 = Normalize(v1);
	Vector3 e2 = Normalize(v2);
	float an = std::acosf(Dot(v1, v2) * (1.0f / (Length(v1) * Length(v2))));
	if (an > 0.0f || an < 180.0f) {
		Vector3 v1e = Multiply(std::sinf((1 - t) * an) / std::sinf(an), e1);
		Vector3 v2e = Multiply(std::sinf(t * an) / std::sinf(an), e2);
		Vector3 result = Multiply(s, Add(v1e, v2e));
		return result;
	}
	return v1;
}
