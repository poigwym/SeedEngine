
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
