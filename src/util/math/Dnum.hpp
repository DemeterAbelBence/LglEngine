#pragma once

#include <GLM/glm.hpp>

#include <gl/BaseDefs.hpp>

namespace lgl
{
	struct LGL_API Dnum {
		float val;
		glm::vec3 der;

		Dnum(float _val, glm::vec3 _der = glm::vec3(0)) { val = _val, der = _der; }
		Dnum operator+(Dnum r) { return Dnum(val + r.val, der + r.der); }
		Dnum operator*(Dnum r) { return Dnum(val * r.val, val * r.der + der * r.val); }
		Dnum operator/(Dnum r) { return Dnum(val / r.val, (der * r.val - val * r.der) / r.val / r.val); }
	};

	LGL_API Dnum _Sin(Dnum r);
	LGL_API Dnum _Cos(Dnum r);
	LGL_API Dnum _Log(Dnum r);
	LGL_API Dnum _Exp(Dnum r);
	LGL_API Dnum _Sqr(Dnum r);
}