/*
** Jo Sega Saturn Engine
** Copyright (c) 2012-2020, Johannes Fetz (johannesfetz@gmail.com)
** All rights reserved.
**
** Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are met:
**     * Redistributions of source code must retain the above copyright
**       notice, this list of conditions and the following disclaimer.
**     * Redistributions in binary form must reproduce the above copyright
**       notice, this list of conditions and the following disclaimer in the
**       documentation and/or other materials provided with the distribution.
**     * Neither the name of the Johannes Fetz nor the
**       names of its contributors may be used to endorse or promote products
**       derived from this software without specific prior written permission.
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
** ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
** WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
** DISCLAIMED. IN NO EVENT SHALL Johannes Fetz BE LIABLE FOR ANY
** DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
** (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
** LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
** ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
** SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/
/** @file 3d.h
 *  @author Johannes Fetz
 *
 *  @brief Jo Engine 3D basics
 *  @bug No known bugs.
 *  @todo Mesh loader, Gouraud shading and advanced 3D processing
 */

#ifndef __JO_3D_H__
# define __JO_3D_H__

#ifdef JO_COMPILE_WITH_3D_SUPPORT

 /** @brief Plane vertice table */
# define JO_3D_PLANE_VERTICES(SIZE) \
{ \
  { -SIZE, -SIZE, 0 }, \
  { SIZE, -SIZE, 0 }, \
  { SIZE, SIZE, 0 }, \
  { -SIZE, SIZE, 0 } \
}

/** @brief Triangle vertice table */
# define JO_3D_TRIANGLE_VERTICES(SIZE) \
{ \
  { -SIZE, -SIZE, 0 }, \
  { SIZE, -SIZE, 0 }, \
  { SIZE, SIZE, 0 } \
}

/** @brief Cube vertice table */
# define JO_3D_CUBE_VERTICES(SIZE) \
{ \
  { -SIZE, -SIZE, SIZE }, \
  { SIZE, -SIZE, SIZE }, \
  { SIZE, SIZE, SIZE }, \
  { -SIZE, SIZE, SIZE }, \
  { -SIZE, -SIZE, -SIZE }, \
  { -SIZE, -SIZE, SIZE }, \
  { -SIZE, SIZE, SIZE }, \
  { -SIZE, SIZE, -SIZE }, \
  { SIZE, -SIZE, -SIZE }, \
  { -SIZE, -SIZE, -SIZE }, \
  { -SIZE, SIZE, -SIZE }, \
  { SIZE, SIZE, -SIZE }, \
  { SIZE, -SIZE, SIZE }, \
  { SIZE, -SIZE, -SIZE }, \
  { SIZE, SIZE, -SIZE }, \
  { SIZE, SIZE, SIZE }, \
  { -SIZE, -SIZE, -SIZE }, \
  { SIZE, -SIZE, -SIZE }, \
  { SIZE, -SIZE, SIZE }, \
  { -SIZE, -SIZE, SIZE }, \
  { -SIZE, SIZE, SIZE }, \
  { SIZE, SIZE, SIZE }, \
  { SIZE, SIZE, -SIZE }, \
  { -SIZE, SIZE, -SIZE } \
}

/** @brief Vertice structure */
typedef jo_pos3D    jo_vertice;

/** @brief Camera structure */
typedef struct
{
    union
    {
        FIXED       viewpoint[XYZ];
        jo_pos3D    viewpoint_pos;
    };
    union
    {
        FIXED       target[XYZ];
        jo_pos3D    target_pos;
    };
    ANGLE           z_angle;
}                   jo_camera;

/** @brief Quadrilateral shape structure */
typedef struct
{
    PDATA       data;
    ATTR        attribute;
}               jo_3d_quad;

/** @brief Mesh structure */
typedef struct
{
    PDATA       data;
}               jo_3d_mesh;

/** @brief Sprite quads (internal engine usage)
 *  @warning MC Hammer: don't touch this
 */
extern jo_3d_quad** __jo_sprite_quad;
bool    jo_3d_create_sprite_quad(const int sprite_id);

#if JO_COMPILE_USING_SGL

/*
 ██████╗ █████╗ ███╗   ███╗███████╗██████╗  █████╗
██╔════╝██╔══██╗████╗ ████║██╔════╝██╔══██╗██╔══██╗
██║     ███████║██╔████╔██║█████╗  ██████╔╝███████║
██║     ██╔══██║██║╚██╔╝██║██╔══╝  ██╔══██╗██╔══██║
╚██████╗██║  ██║██║ ╚═╝ ██║███████╗██║  ██║██║  ██║
 ╚═════╝╚═╝  ╚═╝╚═╝     ╚═╝╚══════╝╚═╝  ╚═╝╚═╝  ╚═╝

*/

/** @brief Set the viewpoint of the camera (where the cameraman is)
 *  @param cam Pointer to a camera struct
 *  @param x X coord
 *  @param y Y coord
 *  @param z Z coord
 */
static  __jo_force_inline void      jo_3d_camera_set_viewpoint(jo_camera* const cam, const int x, const int y, const int z)
{
    cam->viewpoint_pos.x = JO_MULT_BY_65536(x);
    cam->viewpoint_pos.y = JO_MULT_BY_65536(y);
    cam->viewpoint_pos.z = JO_MULT_BY_65536(z);
}

/** @brief Set the target of the camera (where the scene is located)
 *  @param cam Pointer to a camera struct
 *  @param x X coord
 *  @param y Y coord
 *  @param z Z coord
 */
static  __jo_force_inline void      jo_3d_camera_set_target(jo_camera* const cam, const int x, const int y, const int z)
{
    cam->target_pos.x = JO_MULT_BY_65536(x);
    cam->target_pos.y = JO_MULT_BY_65536(y);
    cam->target_pos.z = JO_MULT_BY_65536(z);
}

/** @brief Set the angle of the camera
 *  @param cam Pointer to a camera struct
 *  @param angle Z-angle in degree
 */
static  __jo_force_inline void      jo_3d_camera_set_z_angle(jo_camera* const cam, const int angle)
{
    cam->z_angle = jo_DEGtoANG_int(angle);
}

/** @brief Initialize the camera with default values
 *  @param cam Pointer to a camera struct
 */
static  __jo_force_inline void      jo_3d_camera_init(jo_camera* const cam)
{
    jo_3d_camera_set_viewpoint(cam, 0, 0, -150);
    jo_3d_camera_set_target(cam, 0, 0, 0);
    jo_3d_camera_set_z_angle(cam, 0);
}

/** @brief look at the camera
 *  @param cam Pointer to a camera struct
 */
static  __jo_force_inline void      jo_3d_camera_look_at(jo_camera* const cam)
{
    slLookAt(cam->viewpoint, cam->target, cam->z_angle);
}

/*
███████╗████████╗ █████╗ ████████╗██╗███████╗████████╗██╗ ██████╗███████╗
██╔════╝╚══██╔══╝██╔══██╗╚══██╔══╝██║██╔════╝╚══██╔══╝██║██╔════╝██╔════╝
███████╗   ██║   ███████║   ██║   ██║███████╗   ██║   ██║██║     ███████╗
╚════██║   ██║   ██╔══██║   ██║   ██║╚════██║   ██║   ██║██║     ╚════██║
███████║   ██║   ██║  ██║   ██║   ██║███████║   ██║   ██║╚██████╗███████║
╚══════╝   ╚═╝   ╚═╝  ╚═╝   ╚═╝   ╚═╝╚══════╝   ╚═╝   ╚═╝ ╚═════╝╚══════╝

*/

/** @brief Get polygon count (visible and not visible)
 *  @warning This function must be called after display polygons
 *  @return polygon count
 */
static  __jo_force_inline unsigned int       jo_3d_get_polygon_count(void)
{
    extern Uint16 TotalPolygons;
    return TotalPolygons;
}

/** @brief Get vertice count
 *  @warning This function must be called after display polygons
 *  @return vertice count
 */
static  __jo_force_inline unsigned int       jo_3d_get_vertices_count(void)
{
    extern Uint16 TotalVertices;
    return TotalVertices;
}

/** @brief Get displayed polygon count
 *  @warning This function must be called after display polygons
 *  @return polygon count
 */
static  __jo_force_inline unsigned int       jo_3d_get_displayed_polygon_count(void)
{
    extern Uint16 DispPolygons;
    return DispPolygons;
}

/*
 ██████╗██████╗ ███████╗ █████╗ ████████╗███████╗    ███████╗██╗███╗   ███╗██████╗ ██╗     ███████╗
██╔════╝██╔══██╗██╔════╝██╔══██╗╚══██╔══╝██╔════╝    ██╔════╝██║████╗ ████║██╔══██╗██║     ██╔════╝
██║     ██████╔╝█████╗  ███████║   ██║   █████╗      ███████╗██║██╔████╔██║██████╔╝██║     █████╗
██║     ██╔══██╗██╔══╝  ██╔══██║   ██║   ██╔══╝      ╚════██║██║██║╚██╔╝██║██╔═══╝ ██║     ██╔══╝
╚██████╗██║  ██║███████╗██║  ██║   ██║   ███████╗    ███████║██║██║ ╚═╝ ██║██║     ███████╗███████╗
 ╚═════╝╚═╝  ╚═╝╚══════╝╚═╝  ╚═╝   ╚═╝   ╚══════╝    ╚══════╝╚═╝╚═╝     ╚═╝╚═╝     ╚══════╝╚══════╝

 ██████╗ ██████╗      ██╗███████╗ ██████╗████████╗
██╔═══██╗██╔══██╗     ██║██╔════╝██╔════╝╚══██╔══╝
██║   ██║██████╔╝     ██║█████╗  ██║        ██║
██║   ██║██╔══██╗██   ██║██╔══╝  ██║        ██║
╚██████╔╝██████╔╝╚█████╔╝███████╗╚██████╗   ██║
 ╚═════╝ ╚═════╝  ╚════╝ ╚══════╝ ╚═════╝   ╚═╝

*/

/** @brief Create a mesh programmatically from vertices
 *  @param quad_count Quad count (Quad count == Polygon count)
 *  @param vertices An array of jo_vertice (4 * quad_count)
 *  @param normals An array of jo_pos3D (quad_count)
 *  @return The mesh
 *  @remarks The return type can be casted to (jo_3d_mesh *)
 */
jo_3d_mesh* jo_3d_create_mesh_from_vertices_and_normals(const unsigned int quad_count, jo_vertice* const vertices, jo_pos3D* const normals);

/** @brief Create a mesh programmatically from vertices
 *  @param quad_count Quad count (Quad count == Polygon count)
 *  @param vertices An array of jo_vertice (4 * quad_count)
 *  @return The mesh
 *  @remarks The return type can be casted to (jo_3d_mesh *)
 */
static  __jo_force_inline jo_3d_mesh* jo_3d_create_mesh_from_vertices(const unsigned int quad_count, jo_vertice* const vertices)
{
    return (jo_3d_create_mesh_from_vertices_and_normals(quad_count, vertices, (jo_pos3D*)0));
}

/** @brief Create a mesh programmatically from vertices
 *  @param quad_count Quad count (Quad count == Polygon count)
 *  @return The mesh
 *  @remarks The return type can be casted to (jo_3d_mesh *)
 */
static  __jo_force_inline jo_3d_mesh* jo_3d_create_mesh(const unsigned int quad_count)
{
    return (jo_3d_create_mesh_from_vertices_and_normals(quad_count, (jo_vertice*)0, (jo_pos3D*)0));
}

/** @brief Free a mesh created with jo_3d_create_mesh()
 *  @param mesh The mesh to free
 */
void                        jo_3d_free_mesh(const jo_3d_mesh* const mesh);

/** @brief Create a four vertices polygon (Quadrilateral shape)
 *  @param quad Address to a jo_3d_quad allocated struct
 *  @param vertices Vertices coords (table of 4 jo_vertice)
 */
void                                jo_3d_create_plane(jo_3d_quad* const quad, jo_vertice* const vertices);

/** @brief Create a cube
 *  @param array Address to a 6 jo_3d_quad allocated struct
 *  @param vertices Vertices coords (table of 24 (6x4) jo_vertice)
 */
void                                jo_3d_create_cube(jo_3d_quad* array, jo_vertice* const vertices);

/** @brief Create a triangle
 *  @param quad Address to a jo_3d_quad allocated struct
 *  @param vertices Vertices coords (table of 3 jo_vertice)
 */
void                                jo_3d_create_triangle(jo_3d_quad* const quad, jo_vertice* const vertices);

/** @brief Get sprite quad
 *  @param sprite_id Sprite Id returned by jo_sprite_add(), jo_sprite_add_tga() or jo_sprite_add_image_pack()
 *  @return Sprite quad
 */
static  __jo_force_inline jo_3d_quad* jo_3d_get_sprite_quad(const int sprite_id)
{
    return (__jo_sprite_quad[sprite_id] == (jo_3d_quad*)0 && !jo_3d_create_sprite_quad(sprite_id) ? (jo_3d_quad*)0 : __jo_sprite_quad[sprite_id]);
}

/** @brief Delete sprite quad
 *  @param sprite_id Sprite Id returned by jo_sprite_add(), jo_sprite_add_tga() or jo_sprite_add_image_pack()
 */
void                                    jo_3d_free_sprite_quad(const int sprite_id);

/*
███╗   ███╗███████╗███████╗██╗  ██╗    ████████╗███████╗██╗  ██╗████████╗██╗   ██╗██████╗ ███████╗
████╗ ████║██╔════╝██╔════╝██║  ██║    ╚══██╔══╝██╔════╝╚██╗██╔╝╚══██╔══╝██║   ██║██╔══██╗██╔════╝
██╔████╔██║█████╗  ███████╗███████║       ██║   █████╗   ╚███╔╝    ██║   ██║   ██║██████╔╝█████╗
██║╚██╔╝██║██╔══╝  ╚════██║██╔══██║       ██║   ██╔══╝   ██╔██╗    ██║   ██║   ██║██╔══██╗██╔══╝
██║ ╚═╝ ██║███████╗███████║██║  ██║       ██║   ███████╗██╔╝ ██╗   ██║   ╚██████╔╝██║  ██║███████╗
╚═╝     ╚═╝╚══════╝╚══════╝╚═╝  ╚═╝       ╚═╝   ╚══════╝╚═╝  ╚═╝   ╚═╝    ╚═════╝ ╚═╝  ╚═╝╚══════╝

*/

/** @brief Set a texture on one polygon on the mesh
 *  @param mesh Address to a jo_3d_mesh allocated struct
 *  @param sprite_id Sprite ID
 *  @param index polygon index on the mesh
 */
void                                jo_3d_set_mesh_polygon_texture(jo_3d_mesh* const mesh, const int sprite_id, const unsigned int index);

/** @brief Set a texture on the mesh
 *  @param mesh Address to a jo_3d_mesh allocated struct
 *  @param sprite_id Sprite ID
 */
void                                jo_3d_set_mesh_texture(jo_3d_mesh* const mesh, const int sprite_id);

/** @brief Set a texture on the quadrilateral
 *  @param quad Address to a jo_3d_quad allocated struct
 *  @param sprite_id Sprite ID
 */
static  __jo_force_inline void      jo_3d_set_texture(jo_3d_quad* const quad, const int sprite_id)
{
    jo_3d_set_mesh_polygon_texture((jo_3d_mesh*)quad, sprite_id, 0);
}

/** @brief Set wireframe mode on the mesh
 *  @param mesh Address to a jo_3d_mesh allocated struct
 *  @param index polygon index on the mesh
 *  @param wireframe True to enable wireframe mode
 */
void                        jo_3d_set_mesh_polygon_wireframe(jo_3d_mesh* const mesh, const unsigned int index, bool wireframe);

/*
   8 BITS PALETTE HANDLING
*/

/** @brief Set a palette on the mesh
 *  @param mesh Address to a jo_3d_mesh allocated struct
 *  @param index polygon index on the mesh
 *  @param palette_id Palette id from jo_create_palette()
 */
static  __jo_force_inline void      jo_3d_set_mesh_polygon_palette(jo_3d_mesh* const mesh, const int palette_id, const unsigned int index)
{
    mesh->data.attbl[index].colno = JO_MULT_BY_256(palette_id);
}

/** @brief Set a palette on one polygon on the mesh
 *  @param mesh Address to a jo_3d_mesh allocated struct
 *  @param palette_id Palette id from jo_create_palette()
 */
static  __jo_force_inline void      jo_3d_set_mesh_palette(jo_3d_mesh* const mesh, const int palette_id)
{
    for (unsigned int i = 0; i < mesh->data.nbPolygon; ++i)
        jo_3d_set_mesh_polygon_palette(mesh, palette_id, i);
}

/** @brief Set a palette on the quadrilateral
 *  @param quad Address to a jo_3d_quad allocated struct
 *  @param palette_id Palette id from jo_create_palette()
 */
static  __jo_force_inline void      jo_3d_set_palette(jo_3d_quad* const quad, const int palette_id)
{
    quad->attribute.colno = JO_MULT_BY_256(palette_id);
}

/** @brief Set a palette on a sprite (3D)
 *  @param sprite_id Sprite Id returned by jo_sprite_add(), jo_sprite_add_tga() or jo_sprite_add_image_pack()
 *  @param palette_id Palette id from jo_create_palette()
 *  @return true if succeeded otherwise false
 */
static  __jo_force_inline bool      jo_3d_set_sprite_palette(const int sprite_id, const int palette_id)
{
    if (__jo_sprite_quad[sprite_id] == JO_NULL && !jo_3d_create_sprite_quad(sprite_id)) return (false);
    jo_3d_set_palette(__jo_sprite_quad[sprite_id], palette_id);
    return (true);
}

/*
███╗   ███╗███████╗███████╗██╗  ██╗    ██╗   ██╗███████╗██████╗ ████████╗██╗ ██████╗███████╗       ██╗       ███╗   ██╗ ██████╗ ██████╗ ███╗   ███╗ █████╗ ██╗
████╗ ████║██╔════╝██╔════╝██║  ██║    ██║   ██║██╔════╝██╔══██╗╚══██╔══╝██║██╔════╝██╔════╝       ██║       ████╗  ██║██╔═══██╗██╔══██╗████╗ ████║██╔══██╗██║
██╔████╔██║█████╗  ███████╗███████║    ██║   ██║█████╗  ██████╔╝   ██║   ██║██║     █████╗      ████████╗    ██╔██╗ ██║██║   ██║██████╔╝██╔████╔██║███████║██║
██║╚██╔╝██║██╔══╝  ╚════██║██╔══██║    ╚██╗ ██╔╝██╔══╝  ██╔══██╗   ██║   ██║██║     ██╔══╝      ██╔═██╔═╝    ██║╚██╗██║██║   ██║██╔══██╗██║╚██╔╝██║██╔══██║██║
██║ ╚═╝ ██║███████╗███████║██║  ██║     ╚████╔╝ ███████╗██║  ██║   ██║   ██║╚██████╗███████╗    ██████║      ██║ ╚████║╚██████╔╝██║  ██║██║ ╚═╝ ██║██║  ██║███████╗
╚═╝     ╚═╝╚══════╝╚══════╝╚═╝  ╚═╝      ╚═══╝  ╚══════╝╚═╝  ╚═╝   ╚═╝   ╚═╝ ╚═════╝╚══════╝    ╚═════╝      ╚═╝  ╚═══╝ ╚═════╝ ╚═╝  ╚═╝╚═╝     ╚═╝╚═╝  ╚═╝╚══════╝
*/

/** @brief Set vertice position in mesh
 *  @param mesh Address to a jo_3d_mesh allocated struct
 *  @param x X position
 *  @param y Y position
 *  @param z Z position
 *  @param index vertice index on the mesh (4 vertices for each quad)
 */
static  __jo_force_inline void      jo_3d_set_mesh_vertice(jo_3d_mesh* const mesh, const jo_fixed x, const jo_fixed y, const jo_fixed z, const unsigned int index)
{
    mesh->data.pntbl[index][0] = x;
    mesh->data.pntbl[index][1] = y;
    mesh->data.pntbl[index][2] = z;
}

/** @brief Set quad normal in mesh
 *  @param mesh Address to a jo_3d_mesh allocated struct
 *  @param x X position
 *  @param y Y position
 *  @param z Z position
 *  @param index quad index on the mesh
 */
static  __jo_force_inline void      jo_3d_set_mesh_normal(jo_3d_mesh* const mesh, const jo_fixed x, const jo_fixed y, const jo_fixed z, const unsigned int index)
{
    mesh->data.pltbl[index].norm[0] = x;
    mesh->data.pltbl[index].norm[1] = y;
    mesh->data.pltbl[index].norm[2] = z;
}

/*
███╗   ███╗███████╗███████╗██╗  ██╗    ██╗     ██╗ ██████╗ ██╗  ██╗████████╗
████╗ ████║██╔════╝██╔════╝██║  ██║    ██║     ██║██╔════╝ ██║  ██║╚══██╔══╝
██╔████╔██║█████╗  ███████╗███████║    ██║     ██║██║  ███╗███████║   ██║
██║╚██╔╝██║██╔══╝  ╚════██║██╔══██║    ██║     ██║██║   ██║██╔══██║   ██║
██║ ╚═╝ ██║███████╗███████║██║  ██║    ███████╗██║╚██████╔╝██║  ██║   ██║
╚═╝     ╚═╝╚══════╝╚══════╝╚═╝  ╚═╝    ╚══════╝╚═╝ ╚═════╝ ╚═╝  ╚═╝   ╚═╝

*/

/** @brief Use a light source
 *  @param x X position of the light source
 *  @param y Y position of the light source
 *  @param z Z position of the light source
 */
static  __jo_force_inline void      jo_3d_light(const int x, const int y, const int z)
{
    FIXED p[XYZ];
    p[X] = JO_MULT_BY_65536(x);
    p[Y] = JO_MULT_BY_65536(y);
    p[Z] = JO_MULT_BY_65536(z);
    slLight(p);
}

/** @brief Enable/Disable light on one polygon on the mesh
 *  @param mesh Address to a jo_3d_mesh allocated struct
 *  @param enabled true to enable light support
 *  @param index polygon index on the mesh
 */
void                                jo_3d_set_mesh_polygon_light(jo_3d_mesh* const mesh, const bool enabled, const unsigned int index);

/** @brief Enable/Disable light on the mesh
 *  @param mesh Address to a jo_3d_mesh allocated struct
 *  @param enabled true to enable light support
 */
void                                jo_3d_set_mesh_light(jo_3d_mesh* const mesh, const bool enabled);

/** @brief Enable/Disable light on the quad
 *  @param quad Address to a jo_3d_quad allocated struct
 *  @param enabled true to enable light support
 */
static  __jo_force_inline void      jo_3d_set_light(jo_3d_quad* const quad, const bool enabled)
{
    jo_3d_set_mesh_polygon_light((jo_3d_mesh*)quad, enabled, 0);
}

/*
████████╗██████╗  █████╗ ███╗   ██╗███████╗██████╗  █████╗ ██████╗ ███████╗███╗   ██╗ ██████╗██╗   ██╗
╚══██╔══╝██╔══██╗██╔══██╗████╗  ██║██╔════╝██╔══██╗██╔══██╗██╔══██╗██╔════╝████╗  ██║██╔════╝╚██╗ ██╔╝
   ██║   ██████╔╝███████║██╔██╗ ██║███████╗██████╔╝███████║██████╔╝█████╗  ██╔██╗ ██║██║      ╚████╔╝
   ██║   ██╔══██╗██╔══██║██║╚██╗██║╚════██║██╔═══╝ ██╔══██║██╔══██╗██╔══╝  ██║╚██╗██║██║       ╚██╔╝
   ██║   ██║  ██║██║  ██║██║ ╚████║███████║██║     ██║  ██║██║  ██║███████╗██║ ╚████║╚██████╗   ██║
   ╚═╝   ╚═╝  ╚═╝╚═╝  ╚═╝╚═╝  ╚═══╝╚══════╝╚═╝     ╚═╝  ╚═╝╚═╝  ╚═╝╚══════╝╚═╝  ╚═══╝ ╚═════╝   ╚═╝

*/

/** @brief Enable/Disable transparency on one polygon on the mesh
 *  @param mesh Address to a jo_3d_mesh allocated struct
 *  @param enabled true to enable transparency
 *  @param index polygon index on the mesh
 */
void                                jo_3d_set_mesh_polygon_transparency(jo_3d_mesh* const mesh, const bool enabled, const unsigned int index);

/** @brief Enable/Disable transparency on the mesh
 *  @param mesh Address to a jo_3d_mesh allocated struct
 *  @param enabled true to enable transparency
 */
void                                jo_3d_set_mesh_transparency(jo_3d_mesh* const mesh, const bool enabled);

/** @brief Enable/Disable transparency on the quadrilateral
 *  @param quad Address to a jo_3d_quad allocated struct
 *  @param enabled true to enable transparency
 */
static  __jo_force_inline void      jo_3d_set_transparency(jo_3d_quad* const quad, const bool enabled)
{
    jo_3d_set_mesh_polygon_transparency((jo_3d_mesh*)quad, enabled, 0);
}

/******************************************************
*              SCREEN DOORS TRANSPARENCY
*******************************************************/

/** @brief Enable/Disable screen doors transparency on one polygon on the mesh
 *  @param mesh Address to a jo_3d_mesh allocated struct
 *  @param enabled true to enable screen doors transparency
 *  @param index polygon index on the mesh
 */
void                                jo_3d_set_mesh_polygon_screen_doors(jo_3d_mesh* const mesh, const bool enabled, const unsigned int index);

/** @brief Enable/Disable screen doors transparency on the mesh
 *  @param mesh Address to a jo_3d_mesh allocated struct
 *  @param enabled true to enable screen doors transparency
 */
void                                jo_3d_set_mesh_screen_doors(jo_3d_mesh* const mesh, const bool enabled);

/** @brief Enable/Disable screen doors transparency on the quad
 *  @param quad Address to a jo_3d_quad allocated struct
 *  @param enabled true to enable screen doors transparency
 */
static  __jo_force_inline void      jo_3d_set_screen_doors(jo_3d_quad* const quad, const bool enabled)
{
    jo_3d_set_mesh_polygon_screen_doors((jo_3d_mesh*)quad, enabled, 0);
}

/*
 ██████╗ ██████╗ ██╗      ██████╗ ██████╗ ███████╗
██╔════╝██╔═══██╗██║     ██╔═══██╗██╔══██╗██╔════╝
██║     ██║   ██║██║     ██║   ██║██████╔╝███████╗
██║     ██║   ██║██║     ██║   ██║██╔══██╗╚════██║
╚██████╗╚██████╔╝███████╗╚██████╔╝██║  ██║███████║
 ╚═════╝ ╚═════╝ ╚══════╝ ╚═════╝ ╚═╝  ╚═╝╚══════╝

*/

/** @brief Set the color on one polygon on the mesh with wireframe option
 *  @param mesh Address to a jo_3d_mesh allocated struct
 *  @param color Color
 *  @param index polygon index on the mesh
 *  @param wireframe True to display only the edge of polygons
 */
void                                jo_3d_set_mesh_polygon_color_ex(jo_3d_mesh* const mesh, const jo_color color, const unsigned int index, bool wireframe);

/** @brief Set the color on one polygon on the mesh
 *  @param mesh Address to a jo_3d_mesh allocated struct
 *  @param color Color
 *  @param index polygon index on the mesh
 */
static  __jo_force_inline void      jo_3d_set_mesh_polygon_color(jo_3d_mesh* const mesh, const jo_color color, const unsigned int index)
{
    jo_3d_set_mesh_polygon_color_ex(mesh, color, index, false);
}

/** @brief Set the color on the mesh
 *  @param mesh Address to a jo_3d_mesh allocated struct
 *  @param color Color
 */
void                                jo_3d_set_mesh_color(jo_3d_mesh* const mesh, const jo_color color);

/** @brief Set the color on the quadrilateral
 *  @param quad Address to a jo_3d_quad allocated struct
 *  @param color Color
 */
static  __jo_force_inline void      jo_3d_set_color(jo_3d_quad* const quad, const jo_color color)
{
    jo_3d_set_mesh_polygon_color((jo_3d_mesh*)quad, color, 0);
}

/*
███╗   ███╗ █████╗ ████████╗██████╗ ██╗██╗  ██╗
████╗ ████║██╔══██╗╚══██╔══╝██╔══██╗██║╚██╗██╔╝
██╔████╔██║███████║   ██║   ██████╔╝██║ ╚███╔╝
██║╚██╔╝██║██╔══██║   ██║   ██╔══██╗██║ ██╔██╗
██║ ╚═╝ ██║██║  ██║   ██║   ██║  ██║██║██╔╝ ██╗
╚═╝     ╚═╝╚═╝  ╚═╝   ╚═╝   ╚═╝  ╚═╝╚═╝╚═╝  ╚═╝
*/

/** @brief Push 3D matrix
 */
static  __jo_force_inline void      jo_3d_push_matrix(void)
{
    slPushMatrix();
}

/** @brief Pop 3D matrix
 */
static  __jo_force_inline void      jo_3d_pop_matrix(void)
{
    slPopMatrix();
}

/*
███╗   ███╗ █████╗ ████████╗██████╗ ██╗██╗  ██╗    ██████╗  ██████╗ ████████╗ █████╗ ████████╗██╗ ██████╗ ███╗   ██╗
████╗ ████║██╔══██╗╚══██╔══╝██╔══██╗██║╚██╗██╔╝    ██╔══██╗██╔═══██╗╚══██╔══╝██╔══██╗╚══██╔══╝██║██╔═══██╗████╗  ██║
██╔████╔██║███████║   ██║   ██████╔╝██║ ╚███╔╝     ██████╔╝██║   ██║   ██║   ███████║   ██║   ██║██║   ██║██╔██╗ ██║
██║╚██╔╝██║██╔══██║   ██║   ██╔══██╗██║ ██╔██╗     ██╔══██╗██║   ██║   ██║   ██╔══██║   ██║   ██║██║   ██║██║╚██╗██║
██║ ╚═╝ ██║██║  ██║   ██║   ██║  ██║██║██╔╝ ██╗    ██║  ██║╚██████╔╝   ██║   ██║  ██║   ██║   ██║╚██████╔╝██║ ╚████║
╚═╝     ╚═╝╚═╝  ╚═╝   ╚═╝   ╚═╝  ╚═╝╚═╝╚═╝  ╚═╝    ╚═╝  ╚═╝ ╚═════╝    ╚═╝   ╚═╝  ╚═╝   ╚═╝   ╚═╝ ╚═════╝ ╚═╝  ╚═══╝

*/

/** @brief Rotate 3D matrix using degree
 *  @param x X Angle in degree
 *  @param y Y Angle in degree
 *  @param z Z Angle in degree
 */
static  __jo_force_inline void      jo_3d_rotate_matrix(short x, short y, short z)
{
    slRotX(jo_DEGtoANG_int(x));
    slRotY(jo_DEGtoANG_int(y));
    slRotZ(jo_DEGtoANG_int(z));
}

/** @brief Rotate 3D matrix using degree (X axis)
 *  @param x X Angle in degree
 */
static  __jo_force_inline void      jo_3d_rotate_matrix_x(short x)
{
    slRotX(jo_DEGtoANG_int(x));
}

/** @brief Rotate 3D matrix using degree (Y axis)
 *  @param y Y Angle in degree
 */
static  __jo_force_inline void      jo_3d_rotate_matrix_y(short y)
{
    slRotY(jo_DEGtoANG_int(y));
}

/** @brief Rotate 3D matrix using degree (Z axis)
 *  @param z Z Angle in degree
 */
static  __jo_force_inline void      jo_3d_rotate_matrix_z(short z)
{
    slRotZ(jo_DEGtoANG_int(z));
}

/** @brief Rotate 3D matrix using radian
 *  @param x X Angle in radian
 *  @param y Y Angle in radian
 *  @param z Z Angle in radian
 */
static  __jo_force_inline void      jo_3d_rotate_matrix_rad(float x, float y, float z)
{
    slRotX(RADtoANG(x));
    slRotY(RADtoANG(y));
    slRotZ(RADtoANG(z));
}

/** @brief Rotate 3D matrix using radian (X axis)
 *  @param x X Angle in radian
 */
static  __jo_force_inline void      jo_3d_rotate_matrix_rad_x(float x)
{
    slRotX(RADtoANG(x));
}

/** @brief Rotate 3D matrix using radian (Y axis)
 *  @param y Y Angle in radian
 */
static  __jo_force_inline void      jo_3d_rotate_matrix_rad_y(float y)
{
    slRotY(RADtoANG(y));
}

/** @brief Rotate 3D matrix using radian (Z axis)
 *  @param z Z Angle in radian
 */
static  __jo_force_inline void      jo_3d_rotate_matrix_rad_z(float z)
{
    slRotZ(RADtoANG(z));
}

/*
███╗   ███╗ █████╗ ████████╗██████╗ ██╗██╗  ██╗    ████████╗██████╗  █████╗ ███╗   ██╗███████╗██╗      █████╗ ████████╗██╗ ██████╗ ███╗   ██╗
████╗ ████║██╔══██╗╚══██╔══╝██╔══██╗██║╚██╗██╔╝    ╚══██╔══╝██╔══██╗██╔══██╗████╗  ██║██╔════╝██║     ██╔══██╗╚══██╔══╝██║██╔═══██╗████╗  ██║
██╔████╔██║███████║   ██║   ██████╔╝██║ ╚███╔╝        ██║   ██████╔╝███████║██╔██╗ ██║███████╗██║     ███████║   ██║   ██║██║   ██║██╔██╗ ██║
██║╚██╔╝██║██╔══██║   ██║   ██╔══██╗██║ ██╔██╗        ██║   ██╔══██╗██╔══██║██║╚██╗██║╚════██║██║     ██╔══██║   ██║   ██║██║   ██║██║╚██╗██║
██║ ╚═╝ ██║██║  ██║   ██║   ██║  ██║██║██╔╝ ██╗       ██║   ██║  ██║██║  ██║██║ ╚████║███████║███████╗██║  ██║   ██║   ██║╚██████╔╝██║ ╚████║
╚═╝     ╚═╝╚═╝  ╚═╝   ╚═╝   ╚═╝  ╚═╝╚═╝╚═╝  ╚═╝       ╚═╝   ╚═╝  ╚═╝╚═╝  ╚═╝╚═╝  ╚═══╝╚══════╝╚══════╝╚═╝  ╚═╝   ╚═╝   ╚═╝ ╚═════╝ ╚═╝  ╚═══╝

*/

/** @brief Translate 3D matrix (using floating numbers)
 *  @param x X Position
 *  @param y Y Position
 *  @param z Z Position
 */
static  __jo_force_inline void      jo_3d_translate_matrixf(float x, float y, float z)
{
    slTranslate(toFIXED(x), toFIXED(y), toFIXED(z));
}

/** @brief Translate 3D matrix
 *  @param x X Position
 *  @param y Y Position
 *  @param z Z Position
 */
static  __jo_force_inline void      jo_3d_translate_matrix_fixed(jo_fixed x, jo_fixed y, jo_fixed z)
{
    slTranslate(x, y, z);
}

/** @brief Translate 3D matrix
 *  @param x X Position
 *  @param y Y Position
 *  @param z Z Position
 */
static  __jo_force_inline void      jo_3d_translate_matrix(int x, int y, int z)
{
    slTranslate(jo_int2fixed(x), jo_int2fixed(y), jo_int2fixed(z));
}

/** @brief Translate 3D matrix (X axis)
 *  @param x X Position
 */
static  __jo_force_inline void      jo_3d_translate_matrix_x(int x)
{
    slTranslate(jo_int2fixed(x), 0, 0);
}

/** @brief Translate 3D matrix (Y axis)
 *  @param y Y Position
 */
static  __jo_force_inline void      jo_3d_translate_matrix_y(int y)
{
    slTranslate(0, jo_int2fixed(y), 0);
}

/** @brief Translate 3D matrix (Z axis)
 *  @param z Z Position
 */
static  __jo_force_inline void      jo_3d_translate_matrix_z(int z)
{
    slTranslate(0, 0, jo_int2fixed(z));
}

/*
██████╗ ██████╗  █████╗ ██╗    ██╗██╗███╗   ██╗ ██████╗
██╔══██╗██╔══██╗██╔══██╗██║    ██║██║████╗  ██║██╔════╝
██║  ██║██████╔╝███████║██║ █╗ ██║██║██╔██╗ ██║██║  ███╗
██║  ██║██╔══██╗██╔══██║██║███╗██║██║██║╚██╗██║██║   ██║
██████╔╝██║  ██║██║  ██║╚███╔███╔╝██║██║ ╚████║╚██████╔╝
╚═════╝ ╚═╝  ╚═╝╚═╝  ╚═╝ ╚══╝╚══╝ ╚═╝╚═╝  ╚═══╝ ╚═════╝

*/

/** @brief Draw a quadrilateral
 *  @param quad Address to a jo_3d_quad allocated struct
 */
static  __jo_force_inline void      jo_3d_draw(jo_3d_quad* const quad)
{
    slPutPolygon(&quad->data);
}

/** @brief Draw a mesh
 *  @param mesh Address to a jo_3d_mesh allocated struct
 */
static  __jo_force_inline void      jo_3d_mesh_draw(jo_3d_mesh* const mesh)
{
    slPutPolygon(&mesh->data);
}

/** @brief Get polygon count on the mesh
 *  @param mesh Address to a jo_3d_mesh allocated struct
 *  @return polygon count
 */
static  __jo_force_inline unsigned int       jo_3d_get_mesh_polygon_count(jo_3d_mesh* const mesh)
{
    return mesh->data.nbPolygon;
}

/** @brief Internal usage for polygon display
 *  @warning Don't use it
 */
static __jo_force_inline void       __jo_poylgon_draw_iterator(jo_node* node)
{
    jo_3d_draw((jo_3d_quad*)node->data.ptr);
}

/** @brief Draw a list of quadrilateral
 *  @param list List of quadrilateral
 */
static  __jo_force_inline void      jo_3d_draw_list(jo_list* const list)
{
    jo_list_foreach(list, __jo_poylgon_draw_iterator);
}

/** @brief Draw an array of quadrilateral
 *  @param array Array of quadrilateral
 *  @param array_size Size of the array
 */
static  __jo_force_inline void      jo_3d_draw_array(jo_3d_quad* const array, int array_size)
{
    while (--array_size >= 0) jo_3d_draw(array + array_size);
}

/** @brief Draw a sprite
 *  @param sprite_id Sprite Id returned by jo_sprite_add(), jo_sprite_add_tga() or jo_sprite_add_image_pack()
 *  @return true if succeeded otherwise false
 */
static  __jo_force_inline bool      jo_3d_draw_sprite(const int sprite_id)
{
    if (__jo_sprite_quad[sprite_id] == JO_NULL && !jo_3d_create_sprite_quad(sprite_id)) return (false);
	
    jo_3d_draw(__jo_sprite_quad[sprite_id]);

    return (true);
}

/** @brief Draw a sprite
 *  @param sprite_id Sprite Id returned by jo_sprite_add(), jo_sprite_add_tga() or jo_sprite_add_image_pack()
 *  @param x X coordinate
 *  @param y Y coordinate
 *  @param z Z coordinate
 */
static  __jo_force_inline void      jo_3d_draw_sprite_at(const int sprite_id, const int x, const int y, const int z)
{
    jo_3d_push_matrix();
    jo_3d_translate_matrix(x, y, z);
    jo_3d_draw_sprite(sprite_id);
    jo_3d_pop_matrix();
}

/** @brief Draw billboard
 *  @param sprite_id Sprite Id returned by jo_sprite_add(), jo_sprite_add_tga() or jo_sprite_add_image_pack()
 *  @param x X coordinate
 *  @param y Y coordinate
 *  @param z Z coordinate
 */
static  __jo_force_inline void      jo_3d_draw_billboard(const int sprite_id, const int x, const int y, const int z)
{
    extern jo_pos3D __jo_sprite_pos;
    __internal_jo_sprite_set_position3D(x, y, z);
    jo_sprite_draw(sprite_id, &__jo_sprite_pos, true, true);
}

/** @brief Draw billboard with a specific scale
 *  @param sprite_id Sprite Id returned by jo_sprite_add(), jo_sprite_add_tga() or jo_sprite_add_image_pack()
 *  @param x X coordinate
 *  @param y Y coordinate
 *  @param z Z coordinate
 *  @param scale Scale
 */
static  __jo_force_inline void      jo_3d_draw_scaled_billboard(const int sprite_id, const int x, const int y, const int z, const int scale)
{
    if (scale == 1)
        jo_3d_draw_billboard(sprite_id, x, y, z);
    else
    {
        extern jo_pos3D __jo_sprite_pos;
        __internal_jo_sprite_set_position3D(x, y, z);
        jo_fixed previous_scale_x = __jo_sprite_attributes.fixed_scale_x;
        jo_fixed previous_scale_y = __jo_sprite_attributes.fixed_scale_y;
        __jo_sprite_attributes.fixed_scale_x = jo_int2fixed(scale);
        __jo_sprite_attributes.fixed_scale_y = jo_int2fixed(scale);
        jo_sprite_draw(sprite_id, &__jo_sprite_pos, true, true);
        __jo_sprite_attributes.fixed_scale_x = previous_scale_x;
        __jo_sprite_attributes.fixed_scale_y = previous_scale_y;
    }
}

/*
███████╗ ██████╗ █████╗ ██╗     ███████╗
██╔════╝██╔════╝██╔══██╗██║     ██╔════╝
███████╗██║     ███████║██║     █████╗
╚════██║██║     ██╔══██║██║     ██╔══╝
███████║╚██████╗██║  ██║███████╗███████╗
╚══════╝ ╚═════╝╚═╝  ╚═╝╚══════╝╚══════╝

*/

/** @brief Change scale using floating number
 *  @param x X Scale
 *  @param y Y Scale
 *  @param z Z Scale
 */
static  __jo_force_inline void      jo_3d_set_scalef(const float x, const float y, const float z)
{
    slScale(toFIXED(x), toFIXED(y), toFIXED(z));
}

/** @brief Change scale
 *  @param x X Scale
 *  @param y Y Scale
 *  @param z Z Scale
 */
static  __jo_force_inline void      jo_3d_set_scale(const int x, const int y, const int z)
{
    slScale(JO_MULT_BY_65536(x), JO_MULT_BY_65536(y), JO_MULT_BY_65536(z));
}

/** @brief Change scale using fixed number
 *  @param x X Scale
 *  @param y Y Scale
 *  @param z Z Scale
 */
static  __jo_force_inline void      jo_3d_set_scale_fixed(const jo_fixed x, const jo_fixed y, const jo_fixed z)
{
    slScale(x, y, z);
}

/** @brief Restore default scale for every 3d model displayed after this call
 */
static  __jo_force_inline void	jo_3d_restore_scale(void)
{
    slScale(JO_NO_ZOOM, JO_NO_ZOOM, JO_NO_ZOOM);
}

/*
██╗    ██╗██╗███╗   ██╗██████╗  ██████╗ ██╗    ██╗       ██╗       ██████╗ ██████╗  ██████╗      ██╗███████╗ ██████╗████████╗██╗ ██████╗ ███╗   ██╗
██║    ██║██║████╗  ██║██╔══██╗██╔═══██╗██║    ██║       ██║       ██╔══██╗██╔══██╗██╔═══██╗     ██║██╔════╝██╔════╝╚══██╔══╝██║██╔═══██╗████╗  ██║
██║ █╗ ██║██║██╔██╗ ██║██║  ██║██║   ██║██║ █╗ ██║    ████████╗    ██████╔╝██████╔╝██║   ██║     ██║█████╗  ██║        ██║   ██║██║   ██║██╔██╗ ██║
██║███╗██║██║██║╚██╗██║██║  ██║██║   ██║██║███╗██║    ██╔═██╔═╝    ██╔═══╝ ██╔══██╗██║   ██║██   ██║██╔══╝  ██║        ██║   ██║██║   ██║██║╚██╗██║
╚███╔███╔╝██║██║ ╚████║██████╔╝╚██████╔╝╚███╔███╔╝    ██████║      ██║     ██║  ██║╚██████╔╝╚█████╔╝███████╗╚██████╗   ██║   ██║╚██████╔╝██║ ╚████║
 ╚══╝╚══╝ ╚═╝╚═╝  ╚═══╝╚═════╝  ╚═════╝  ╚══╝╚══╝     ╚═════╝      ╚═╝     ╚═╝  ╚═╝ ╚═════╝  ╚════╝ ╚══════╝ ╚═════╝   ╚═╝   ╚═╝ ╚═════╝ ╚═╝  ╚═══╝

*/

/** @brief Sets up a window region on the projection surface
 *  @param left
 *  @param top
 *  @param right
 *  @param bottom
 *  @param z_limit
 *  @param center_x
 *  @param center_y
 *  @return false if Failed
 */
static  __jo_force_inline bool      jo_3d_window(const int left, const int top, const int right, const int bottom, const int z_limit, const int center_x, const int center_y)
{
    return slWindow(left, top, right, bottom, z_limit, center_x, center_y) != 0;
}

/** @brief Set the perspective angle
 *  @param angle Angle in degree (from 21 to 159)
 */
static  __jo_force_inline void      jo_3d_perspective_angle(const int angle)
{
#if JO_DEBUG
    if (angle < 10 || angle > 160)
        jo_core_error("Valid angle range is 10 to 160");
#endif
    slPerspective(jo_DEGtoANG_int(angle));
}

/** @brief Set the display level of the viewing volume
 *  @param level Distance from the viewpoint and the projection surface
 */
static  __jo_force_inline void      jo_3d_display_level(const unsigned short level)
{
    slZdspLevel(level);
}

#endif

#endif /* !JO_COMPILE_WITH_3D_SUPPORT */

#endif /* !__JO_3D_H__ */

/*
** END OF FILE
*/
