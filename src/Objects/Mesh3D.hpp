#pragma once

#include <jo\Jo.hpp>
#include "..\Utils\Math\Vec3.hpp"

namespace Objects
{
    struct Mesh3D : public PDATA, JoType<Mesh3D, jo_3d_mesh>
    {
        Vec3* PointTable()
        {
            return reinterpret_cast<Vec3*>(pntbl);
        }

        Mesh3D()
        {
            pntbl = nullptr;
            nbPoint = 0;
            pltbl = nullptr;
            nbPolygon = 0;
            attbl = nullptr;
        }

        Mesh3D(const size_t& pointCount, const size_t& polygonCount)
        {
            pntbl = new POINT[pointCount];
            nbPoint = pointCount;
            pltbl = new POLYGON[polygonCount];
            nbPolygon = polygonCount;
            attbl = new ATTR[polygonCount];
        }

        Mesh3D(const size_t& pointCount, POINT* srcPointTable, const size_t& polygonCount,
            POLYGON* srcPolygonTable, const bool& dmaCopy = true) :
            Mesh3D(pointCount, polygonCount)
        {
            slDMACopy(srcPointTable, pntbl, pointCount * sizeof(POINT));
            slDMACopy(srcPolygonTable, pltbl, polygonCount * sizeof(POLYGON));
        }

        // Move constructor
        Mesh3D(Mesh3D&& other) noexcept
            : PDATA(std::move(other))  // Call base class move constructor
        {
            // Steal resources from the source object
            pntbl = other.pntbl;
            nbPoint = other.nbPoint;
            pltbl = other.pltbl;
            nbPolygon = other.nbPolygon;
            attbl = other.attbl;

            // Reset the source object
            other.pntbl = nullptr;
            other.nbPoint = 0;
            other.pltbl = nullptr;
            other.nbPolygon = 0;
            other.attbl = nullptr;
        }

        // Move assignment operator
        Mesh3D& operator=(Mesh3D&& other) noexcept
        {
            if (this != &other)
            {
                // Steal resources from the source object
                delete[] pntbl;
                pntbl = other.pntbl;
                nbPoint = other.nbPoint;

                delete[] pltbl;
                pltbl = other.pltbl;
                nbPolygon = other.nbPolygon;

                delete[] attbl;
                attbl = other.attbl;

                // Reset the source object
                other.pntbl = nullptr;
                other.nbPoint = 0;
                other.pltbl = nullptr;
                other.nbPolygon = 0;
                other.attbl = nullptr;
            }

            return *this;
        }

        // Destructor
        ~Mesh3D()
        {
            // Release resources
            delete[] pntbl;
            delete[] pltbl;
            delete[] attbl;
        }
    };
}
