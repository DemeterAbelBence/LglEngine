#include "CuboidCollider.hpp"

using namespace lgl;

float CuboidCollider::vertexFaceThreshold = 0.4f;

float CuboidCollider::edgeEdgeThreshold = 0.4f;

void CuboidCollider::createCorners() {
    float w = baseData.width / 2.0f;
    float h = baseData.height / 2.0f;
    float l = baseData.length / 2.0f;

    baseData.points[0] = glm::vec3(-w, -h, l);
    baseData.points[1] = glm::vec3(w, -h, l);
    baseData.points[2] = glm::vec3(w, -h, -l);
    baseData.points[3] = glm::vec3(-w, -h, -l);

    baseData.points[4] = glm::vec3(-w, h, l);
    baseData.points[5] = glm::vec3(w, h, l);
    baseData.points[6] = glm::vec3(w, h, -l);
    baseData.points[7] = glm::vec3(-w, h, -l);

    transData = baseData;
}

CuboidCollider::Side CuboidCollider::createSide(const std::array<UINT, 4>& indices) const {
    std::array<glm::vec3, 4> points;
    glm::vec3 center = glm::vec3(0.0f, 0.0f, 0.0f);
    for (int i = 0; i < indices.size(); ++i) {
        points[i] = baseData.points[indices[i]];
        center += points[i];
    }
    center /= static_cast<float>(indices.size());

    return { points, glm::normalize(center) };
}

void CuboidCollider::createSubdivision(UINT subdivideInto) {
     const float eps = 0.0001f;
     std::vector<glm::vec3> totalSubd;

     auto side = baseData.sides[0];
     glm::vec3 sa = side.points[1] - side.points[0];
     glm::vec3 sb = side.points[3] - side.points[0];
     float la = glm::length(sa);
     float lb = glm::length(sb);
     float da = la / (float)subdivideInto;
     float db = lb / (float)subdivideInto;
     sa = glm::normalize(sa);
     sb = glm::normalize(sb);
     std::vector<glm::vec3> sideSubd;
     for (float i = 0.0f; i < la + eps; i += da) {
         for (float j = 0.0f; j < lb + eps; j += db) {
             glm::vec3 p = side.points[0] + i * sa + j * sb;
             sideSubd.push_back(p);
         }
     }
     totalSubd.insert(totalSubd.begin(), sideSubd.begin(), sideSubd.end());
     for (auto& p : sideSubd) {
         p -= baseData.height * side.normal;
     }
     totalSubd.insert(totalSubd.begin(), sideSubd.begin(), sideSubd.end());

     auto p = side.points;
     std::vector<glm::vec3> edges;
     edges.push_back(p[0]); edges.push_back(p[1]);
     edges.push_back(p[1]); edges.push_back(p[2]);
     edges.push_back(p[2]); edges.push_back(p[3]);
     edges.push_back(p[3]); edges.push_back(p[0]);
     std::vector<glm::vec3> borderSubd;
     for (int i = 0; i < edges.size(); i += 2) {
         glm::vec3 ev = edges[i + 1] - edges[i];
         glm::vec3 en = glm::normalize(ev);
         float el = glm::length(ev);
         float ed = el / (float)subdivideInto;
         float l = el - ed + eps;
         for (float j = 0.0f; j < l; j += ed) {
             glm::vec3 p = edges[i] + j * en;
             borderSubd.push_back(p);
         }
     }

     float dh = baseData.height / (float)subdivideInto;
     for (float i = dh; i < baseData.height - dh + eps; i += dh) {
         for (const auto& b : borderSubd) {
             glm::vec3 p = b - i * side.normal;
             totalSubd.push_back(p);
         }
     }

     baseData.subdivision = totalSubd;
     transData.subdivision = totalSubd;
}

void CuboidCollider::createSides() {
    std::array<UINT, 4> sideIndices[6] =
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

std::vector<glm::vec3> CuboidCollider::getEdgesOf(const CuboidCollider& collider) const {
    auto p = collider.transData.points;
    std::vector<glm::vec3> edges;

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

std::optional<Collider::ContactData> CuboidCollider::calculateEdgeIntersection(glm::vec3 pa, glm::vec3 va, float a, glm::vec3 pb, glm::vec3 vb, float b) const {
    if (va == glm::vec3(0.0f, 0.0f, 0.0f) || vb == glm::vec3(0.0f, 0.0f, 0.0f)) {
        std::cout << "Edge direction vector is zero" << std::endl;
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
        return ContactData {
            glm::vec3((ra + rb) / 2.0f),
            glm::normalize(glm::cross(vb, va)),
            {},
            false,
            {ra, va},
            {rb, vb}
        };
    }
}

std::vector<Collider::ContactData> CuboidCollider::edgeEdgeCollision(const CuboidCollider& collidee) const {
    std::vector<Collider::ContactData> result;
    std::vector<glm::vec3> edgesA = getEdgesOf(*this);
    std::vector<glm::vec3> edgesB = getEdgesOf(collidee);

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
                result.push_back(*contact);
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
        std::vector<Edge> edges;
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

std::vector<Collider::ContactData> CuboidCollider::vertexFaceCollision(const CuboidCollider& collidee) const {
    std::vector<Collider::ContactData> result;

    for (const Side& side : collidee.getTransData().sides) {
        for (const glm::vec3& point : transData.points) {
            if (calculateSideIntersection(point, side)) {
                glm::vec3 sideCenter = calculateSideCenter(side);
				ContactData contact = { point, side.normal, {}, true, {}, {}, sideCenter };
                contact.depth = calculateDepth(collidee, contact);
                result.push_back(contact);
            }
        }
    }

    return result;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - //

CuboidCollider::CuboidCollider(glm::vec3 dimensions) {
    baseData.width = dimensions.x;
    baseData.height = dimensions.y;
    baseData.length = dimensions.z;

    geomType = GeomType::C_CUBOID;

    createCorners();
    createSides();
    createSubdivision(2);
}

CuboidCollider::CuboidCollider(const CuboidCollider& c) {
    geomType = c.geomType;


    baseData = c.baseData;
    transData = c.transData;

    // WHAT THE HEEEEEEELL
    transformation = c.transformation;
}

std::vector<glm::vec3> CuboidCollider::getSideDrawData(UINT sideIndex) {
    std::vector<glm::vec3> result;

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
    glm::vec3 center = glm::vec3(0.0f, 0.0f, 0.0f);
    for (const auto& p : side.points) {
        center += p;
    }
    center /= 4.0f;
    return center;
}

void CuboidCollider::updateTransformations() {
    glm::mat4 M = transformation->makeModelMatrix();
    glm::mat4 MI = transformation->makeModelInverseMatrix();
    glm::vec3 scale = transformation->getScaling();
    
    transData.width = scale.x * baseData.width;
	transData.height = scale.y * baseData.height;
    transData.length = scale.z * baseData.length;

    for (int i = 0; i < transData.points.size(); ++i) {
        transData.points[i] = glm::vec3(glm::vec4(baseData.points[i], 1.0f) * M);
    }

    for (int i = 0; i < transData.subdivision.size(); ++i) {
        transData.subdivision[i] = glm::vec3(glm::vec4(baseData.subdivision[i], 1.0f) * M);
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

std::vector<Collider::ContactData> CuboidCollider::collidesWith(const CuboidCollider& collidee) const {
    auto edgeEdge = edgeEdgeCollision(collidee);
    auto vertexFace = vertexFaceCollision(collidee);
    edgeEdge.insert(edgeEdge.end(), vertexFace.begin(), vertexFace.end());
    return edgeEdge;
}

std::vector<Collider::ContactData> CuboidCollider::collidesWith(const SphereCollider& collidee) const {
    return std::vector<ContactData>();
}

std::vector<Collider::ContactData> CuboidCollider::collidesWith(const TerrainCollider& collidee) const {
    std::vector<ContactData> result;
    auto MI = collidee.getTransformation()->makeModelInverseMatrix();
    auto M = collidee.getTransformation()->makeModelMatrix();
    for (const auto& p : transData.subdivision) {
        glm::vec3 tp = glm::vec3(glm::vec4(p, 1.0f) * MI);
        auto e = collidee.evaluateTerrainFormula(tp.x, tp.z);

        glm::vec3 ep = std::get<0>(e);
        glm::vec3 en = std::get<1>(e);
        glm::vec3 tep = glm::vec3(glm::vec4(ep, 1.0f) * M);
        glm::vec3 ten = glm::vec3(MI * glm::vec4(en, 0.0f));

        if (tep.y > p.y) {
            ContactData contact = { p, glm::normalize(ten), {}, true, {}, {} };
			contact.depth = calculateDepth(collidee, contact);
            result.push_back(contact);
        }
    }
    return result;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - //

std::optional<glm::vec3> CuboidCollider::calculateDepth(const CuboidCollider& collidee, const ContactData& contact) const {
    if (contact.isVertexFace) {
		glm::normalize(contact.normal);
        float faceDot = glm::dot(contact.point - contact.facePoint, contact.normal);
        if (faceDot >= 0.0f) {
            return {};
        }
        else {
            return -contact.normal * faceDot;
        }
    }
    else {
        glm::mat4 RT = collidee.getTransformation()->getR() * collidee.getTransformation()->getT();
        glm::mat4 RTI = glm::inverse(RT);
        glm::vec3 edgePointA = contact.edgeA[0];
        edgePointA = glm::vec3(glm::vec4(edgePointA, 1.0f) * RTI);

        if (glm::abs(edgePointA.x) < collidee.getTransData().width / 2.0f &&
            glm::abs(edgePointA.y) < collidee.getTransData().height / 2.0f &&
            glm::abs(edgePointA.z) < collidee.getTransData().length / 2.0f) {

            return glm::vec3(contact.edgeB[0] - contact.edgeA[0]);
        }
        else {
            return {};
        }
    }
}

std::optional<glm::vec3> CuboidCollider::calculateDepth(const SphereCollider& collidee, const ContactData& contact) const {
    return {};
}

std::optional<glm::vec3> CuboidCollider::calculateDepth(const TerrainCollider& collidee, const ContactData& contact) const {
    auto MI = collidee.getTransformation()->makeModelInverseMatrix();
    auto M = collidee.getTransformation()->makeModelMatrix();

    glm::vec3 tp = glm::vec3(glm::vec4(contact.point, 1.0f) * MI);
    auto e = collidee.evaluateTerrainFormula(tp.x, tp.z);

    glm::vec3 ep = std::get<0>(e);
    glm::vec3 tep = glm::vec3(glm::vec4(ep, 1.0f) * M);

    if (tep.y < contact.point.y) {
        return {};
    }
    else {
        return glm::vec3(tep - contact.point);
    }
}
