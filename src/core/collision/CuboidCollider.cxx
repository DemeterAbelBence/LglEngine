#include "CuboidCollider.hxx"

namespace lgl {
    void CuboidCollider::createCorners() {
        baseData.points[0] = glm::vec3(-0.5f, -0.5f, 0.5f);
        baseData.points[1] = glm::vec3(0.5f, -0.5f, 0.5f);
        baseData.points[2] = glm::vec3(0.5f, -0.5f, -0.5f);
        baseData.points[3] = glm::vec3(-0.5f, -0.5f, -0.5f);

        baseData.points[4] = glm::vec3(-0.5f, 0.5f, 0.5f);
        baseData.points[5] = glm::vec3(0.5f, 0.5f, 0.5f);
        baseData.points[6] = glm::vec3(0.5f, 0.5f, -0.5f);
        baseData.points[7] = glm::vec3(-0.5f, 0.5f, -0.5f);

        transData = baseData;
    }

    CuboidCollider::Side CuboidCollider::createSide(const utl::arr<utl::uint, 4>& indices) const {
        utl::arr<glm::vec3, 4> points;
        glm::vec3 center = glm::nullvec;
        for (int i = 0; i < indices.size(); ++i) {
            points[i] = baseData.points[indices[i]];
            center += points[i];
        }
        center /= static_cast<float>(indices.size());

        return { points, glm::normalize(center) };
    }

    void CuboidCollider::createSides() {
        utl::arr<utl::uint, 4> sideIndices[6] =
        {
            {0, 1, 2, 3},
            {4, 5, 6, 7},

            {0, 3, 7, 4},
            {1, 2, 6, 5},

            {0, 1, 5, 4},
            {3, 2, 6, 7}
        };

        for (int i = 0; i < 6; ++i) {
            baseData.sides[i] = createSide(sideIndices[i]);
            transData.sides[i] = createSide(sideIndices[i]);
        }
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - //

    utl::vec<glm::vec3> CuboidCollider::getEdgesOf(const CuboidCollider& collider) const {
        auto p = collider.transData.points;
        utl::vec<glm::vec3> edges;

        edges.push_back(p[0]); edges.push_back(p[1]);
        edges.push_back(p[1]); edges.push_back(p[2]);
        edges.push_back(p[2]); edges.push_back(p[3]);
        edges.push_back(p[3]); edges.push_back(p[0]);

        edges.push_back(p[4]); edges.push_back(p[5]);
        edges.push_back(p[5]); edges.push_back(p[6]);
        edges.push_back(p[6]); edges.push_back(p[7]);
        edges.push_back(p[7]); edges.push_back(p[4]);

        edges.push_back(p[0]); edges.push_back(p[4]);
        edges.push_back(p[1]); edges.push_back(p[5]);
        edges.push_back(p[2]); edges.push_back(p[6]);
        edges.push_back(p[3]); edges.push_back(p[7]);

        return edges;
    }

    utl::opt<Collider::ContactData> CuboidCollider::calculateEdgeIntersection(
        glm::vec3 pa, glm::vec3 va, float a,
        glm::vec3 pb, glm::vec3 vb, float b) const {

        if (va == glm::nullvec || vb == glm::nullvec) {
            constexpr utl::cstr logText = "Edge direction vector is zero : va = ({:.2f}, {:.2f}, {:.2f}), vb = ({:.2f}, {:.2f}, {:.2f})\n";
            Logger::logIf(false, Logger::LGL_ERROR, logText, va.x, va.y, va.z, vb.x, vb.y, vb.z);
            return {};
        }

        glm::vec3 c = glm::cross(va, vb);
        float dist = glm::length(glm::dot(pb - pa, c)) / glm::length(c);
        if (dist > edgeEdgeThreshold) {
            return {};
        }
        else {
            float PA = glm::dot(pb, va) - glm::dot(pa, va);
            float PB = glm::dot(pa, vb) - glm::dot(pb, vb);
            float VBA = glm::dot(vb, va);
            float VBB = glm::dot(vb, vb);
            float VAA = glm::dot(va, va);
            float beta = (-PB - (PA * VBA) / VAA) / (VBA * VBA / VAA - VBB);
            float alpha = (beta * VBA + PA) / VAA;

            if (!(0 <= alpha && alpha <= a)) {
                return {};
            }
            if (!(0 <= beta && beta <= b)) {
                return {};
            }

            glm::vec3 ra = pa + alpha * va;
            glm::vec3 rb = pb + beta * vb;
            glm::vec3 n = glm::normalize(glm::cross(vb, va));

            if (glm::dot(n, ra - rb) < 0.0f) {
                n = -n;
            }

            return Collider::ContactData{
                .point = glm::vec3((ra + rb) / 2.0f),
                .normal = -n,
                .depth = {},
                .isVertexFace = false,
                .edgeA = {ra, va},
                .edgeB = {rb, vb}
            };
        }
    }

    utl::vec<Collider::ContactData> CuboidCollider::edgeEdgeCollision(const CuboidCollider& collidee) const {
        utl::vec<Collider::ContactData> result;
        utl::vec<glm::vec3> edgesA = getEdgesOf(*this);
        utl::vec<glm::vec3> edgesB = getEdgesOf(collidee);

        for (int i = 0; i < edgesA.size(); i += 2) {
            glm::vec3 vecA = edgesA[i + 1] - edgesA[i];
            float a = glm::length(vecA);

            for (int j = 0; j < edgesB.size(); j += 2) {
                glm::vec3 vecB = edgesB[j + 1] - edgesB[j];
                float b = glm::length(vecB);

                vecA = glm::normalize(vecA);
                vecB = glm::normalize(vecB);

                auto contact = calculateEdgeIntersection(edgesA[i], vecA, a, edgesB[j], vecB, b);
                if (contact.has_value()) {
                    contact->depth = calculateDepth(collidee, *contact);
                    if (contact->depth.has_value()) {
                        result.push_back(*contact);
					}
                }
            }
        }

        return result;
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - //

    bool CuboidCollider::calculateSideIntersection(glm::vec3 point, const Side& side) const {
        float arePerpendicular = glm::dot(side.normal, point - side.points[0]);
        if (glm::abs(arePerpendicular) > vertexFaceThreshold) {
            return false;
        }
        else {
            utl::vec<Edge> edges;
            edges.push_back({ side.points[0], side.points[1] - side.points[0] });
            edges.push_back({ side.points[1], side.points[2] - side.points[1] });
            edges.push_back({ side.points[2], side.points[3] - side.points[2] });
            edges.push_back({ side.points[3], side.points[0] - side.points[3] });

            for (const auto& e : edges) {
                glm::vec3 ev = glm::normalize(e.vector);
                glm::vec3 pv = glm::normalize(point - e.point);
                float d = glm::dot(ev, pv);
                if (!(0.0f <= d && d <= 1.0f)) {
                    return false;
                }
            }
            return true;
        }
    }

    utl::vec<Collider::ContactData> CuboidCollider::vertexFaceCollision(const CuboidCollider& collidee) const {
        utl::vec<Collider::ContactData> result;

        for (const Side& side : collidee.getTransData().sides) {
            for (const glm::vec3& point : transData.points) {
                if (calculateSideIntersection(point, side)) {
                    Collider::ContactData contact = { 
                        .point = point, 
                        .normal = side.normal, 
                        .depth = {},
                        .isVertexFace = true, 
                        .edgeA = {}, 
                        .edgeB = {}, 
                        .facePoint = calculateSideCenter(side)
                    };
					contact.depth = calculateDepth(collidee, contact);
                    if (contact.depth.has_value()) {
                        result.push_back(contact);
                    }
                }
            }
        }

        return result;
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - //

    CuboidCollider::CuboidCollider(glm::vec3 dimensions) {
        geomType = GeomType::C_CUBOID;

        createCorners();
        createSides();
    }

    CuboidCollider::CuboidCollider(const CuboidCollider& c) {
        geomType = c.geomType;

        baseData = c.baseData;
        transData = c.transData;
        
        transformation = c.transformation->clone();
    }

    utl::vec<glm::vec3> CuboidCollider::getSideDrawData(utl::uint sideIndex) {
        utl::vec<glm::vec3> result;

        auto side = transData.sides[sideIndex];
        auto points = side.points;
        glm::vec3 center = calculateSideCenter(side);

        for (auto& p : side.points) {
            p += 0.1f * side.normal;
        }

        result.push_back(center); result.push_back(center + 1.5f * side.normal);

        result.push_back(points[0]); result.push_back(points[1]);
        result.push_back(points[1]); result.push_back(points[2]);
        result.push_back(points[2]); result.push_back(points[3]);
        result.push_back(points[3]); result.push_back(points[0]);

        return result;
    }

    glm::vec3 CuboidCollider::calculateSideCenter(const Side& side) const {
        glm::vec3 center = glm::nullvec;
        for (const auto& p : side.points) {
            center += p;
        }
        center /= 4.0f;
        return center;
    }

    void CuboidCollider::updateTransformations() {
        glm::mat4 M = transformation->makeModelMatrix();
        glm::mat4 MI = transformation->makeModelInverseMatrix();

        for (int i = 0; i < transData.points.size(); ++i) {
            transData.points[i] = glm::vec3(glm::vec4(baseData.points[i], 1.0f) * M);
        }

        for (int i = 0; i < transData.sides.size(); ++i) {
            for (int j = 0; j < transData.sides[i].points.size(); ++j) {
                glm::vec3 newPoint = glm::vec3(glm::vec4(baseData.sides[i].points[j], 1.0f) * M);
                transData.sides[i].points[j] = newPoint;
            }
            transData.sides[i].normal = glm::vec3(MI * glm::vec4(baseData.sides[i].normal, 0.0f));
            transData.sides[i].normal = glm::normalize(transData.sides[i].normal);
        }
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - //

    utl::vec<Collider::ContactData> CuboidCollider::collidesWith(const CuboidCollider& collidee) const {
        auto edgeEdge = edgeEdgeCollision(collidee);
        auto vertexFace = vertexFaceCollision(collidee);
        edgeEdge.insert(edgeEdge.end(), vertexFace.begin(), vertexFace.end());
        return edgeEdge;
    }

    utl::vec<Collider::ContactData> CuboidCollider::collidesWith(const SphereCollider& collidee) const {
        return utl::vec<ContactData>();
    }

    utl::vec<Collider::ContactData> CuboidCollider::collidesWith(const TerrainCollider& collidee) const {
        utl::vec<ContactData> result;
        glm::mat4 MI = collidee.getTransformation()->makeModelInverseMatrix();
        glm::mat4 M = collidee.getTransformation()->makeModelMatrix();
        for (const auto& p : transData.points) {
            glm::vec3 tp = glm::vec3(glm::vec4(p, 1.0f) * MI);
            auto e = collidee.evaluateTerrainFormula(tp.x, tp.z);

            glm::vec3 ep = e.get<0>();
            glm::vec3 en = e.get<1>();
            glm::vec3 tep = glm::vec3(glm::vec4(ep, 1.0f) * M);
            glm::vec3 ten = glm::vec3(MI * glm::vec4(en, 0.0f));

            if (tep.y > p.y) {
                ContactData contact = { 
                    .point = p, 
                    .normal = glm::normalize(ten), 
                    .depth = {}, 
                    .isVertexFace = true, 
                    .edgeA{}, 
                    .edgeB{} 
                };

                contact.depth = calculateDepth(collidee, contact);
                result.push_back(contact);
            }
        }
        return result;
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - //

    utl::opt<glm::vec3> CuboidCollider::calculateDepth(const CuboidCollider& collidee, const ContactData& contact) const {
        if (contact.isVertexFace) {
            float faceDot = glm::dot(contact.point - contact.facePoint, contact.normal);
            if (faceDot >= 0.0f) {
                return {};
            }
            else {
                return -contact.normal * faceDot;
            }
        }
        else {
			// IS EDGE POINT A INSIDE COLLIDEE CUBOID
            bool isInsideCollidee = false;
            glm::mat4 RT1 = collidee.getTransformation()->getR() * collidee.getTransformation()->getT();
            glm::mat4 RTI1 = glm::inverse(RT1);
            glm::vec3 edgePointA = contact.edgeA[0];
            edgePointA = glm::vec3(glm::vec4(edgePointA, 1.0f) * RTI1);
            glm::vec3 scaling1 = collidee.getTransformation()->getScaling();
            if (glm::abs(edgePointA.x) < scaling1.x / 2.0f &&
                glm::abs(edgePointA.y) < scaling1.y / 2.0f &&
                glm::abs(edgePointA.z) < scaling1.z / 2.0f) {
                isInsideCollidee = true;
            }

			// IS EDGE POINT B INSIDE COLLIDER CUBOID
			bool isInsideCollider = false;
			glm::mat4 RT2 = this->getTransformation()->getR() * this->getTransformation()->getT();
			glm::mat4 RTI2 = glm::inverse(RT2);
			glm::vec3 edgePointB = contact.edgeB[0];
			edgePointB = glm::vec3(glm::vec4(edgePointB, 1.0f) * RTI2);
            glm::vec3 scaling2 = this->getTransformation()->getScaling();
            if (glm::abs(edgePointB.x) < scaling2.x / 2.0f &&
                glm::abs(edgePointB.y) < scaling2.y / 2.0f &&
                glm::abs(edgePointB.z) < scaling2.z / 2.0f) {
                isInsideCollider = true;
            }

            if (isInsideCollidee || isInsideCollider) {
                glm::vec3 depthVec = glm::vec3(contact.edgeB[0] - contact.edgeA[0]);
                return isInsideCollidee ? depthVec : -depthVec;
            }
            else {
                return {};
            }
        }
    }

    utl::opt<glm::vec3> CuboidCollider::calculateDepth(const SphereCollider& collidee, const ContactData& contact) const {
        return {};
    }

    utl::opt<glm::vec3> CuboidCollider::calculateDepth(const TerrainCollider& collidee, const ContactData& contact) const {
        glm::mat4 MI = collidee.getTransformation()->makeModelInverseMatrix();
        glm::mat4 M = collidee.getTransformation()->makeModelMatrix();

        glm::vec3 tp = glm::vec3(glm::vec4(contact.point, 1.0f) * MI);
        auto e = collidee.evaluateTerrainFormula(tp.x, tp.z);

        glm::vec3 ep = e.get<0>();
        glm::vec3 tep = glm::vec3(glm::vec4(ep, 1.0f) * M);

        if (tep.y < contact.point.y) {
            return {};
        }
        else {
            return glm::vec3(tep - contact.point);
        }
    }
}