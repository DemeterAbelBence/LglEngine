#include "Dnum.hpp"

lgl::Dnum lgl::_Sin(Dnum r) { return lgl::Dnum(glm::sin(r.val), glm::cos(r.val) * r.der); }
lgl::Dnum lgl::_Cos(Dnum r) { return lgl::Dnum(glm::cos(r.val), -glm::sin(r.val) * r.der); }
lgl::Dnum lgl::_Log(Dnum r) { return lgl::Dnum(glm::log(r.val), r.der / r.val); }
lgl::Dnum lgl::_Exp(Dnum r) { return lgl::Dnum(glm::exp(r.val), glm::exp(r.val) * r.der); }
lgl::Dnum lgl::_Sqr(Dnum r) { return lgl::Dnum(glm::sqrt(r.val), r.der / (2 * glm::sqrt(r.val))); }