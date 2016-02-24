
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


