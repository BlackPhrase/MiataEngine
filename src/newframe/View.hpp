#pragma once

class CView
{
public:
	void Init();
	
	void RenderView();
	
	void UpdatePalette();
	
	float CalcRoll(vec3_t angles, vec3_t velocity);
};