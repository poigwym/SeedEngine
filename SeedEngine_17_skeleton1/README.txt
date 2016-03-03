
---SeedEngine14
add material system
provide normalmap, texturemap
provide dynamic shadow

material file look like this:
material test 
{
	pro_vertex test.hlsl vsmain vs_2_0
	pro_pirxel test.hlsl psmain ps_2_0
	texture 
	{
		tex_normal  aa.jpg
	}
	texture 
	{
		tex_color bb.jpg
	}
}

just add material to GameObject, then the engine automatically render GameObject with this feather!!!





---SeedEngine15_1
begin learning terrain edit, pick, terrain lod.....


---SeedEngine15_2 GeoMipmap
add GeoMipmap terrain, the demo generate a height map manually, and set a random lod in each patch.




--SeedEngine16
>change the solid vertex to generic Vertex, which is made of VertexDeclaration, VertexData, Buffers. The generic
Vertex are prepared for models, Because the vertex format of a model varies from others, and I want to make a 
data driven Material System, then it will fly!!!

>Add Fbx Model loader, OBJ loader. only static mesh.

>Add SubMesh class as a model group, while contain  Material and Geometry. When loading model, add Vertex to 
Geometry,  set Texture to material.

>Fix the TextureManager a bit.  It can load texture from disk, ..... I only make it look simpler.



--SeedEngine17

>add Skeleton animation, hardcode a triangle influenced by 2 vertices, thanks to my generic VertexDeclaration, 
it make me easily add the BlendWeight and BlendIndices to the Vertex. But the incredible fucking thing is
the dx9 D3DXHANDLE of shaderprogram, where it get the array Handle by its name rather by arrayname[size].






