#pragma once

struct IWindow
{
	virtual void SetPos() = 0;
	virtual void GetPos() const = 0;
	
	virtual void SetSize() = 0;
	virtual void GetSize() const = 0;
	
	virtual void SetTitle() = 0;
	virtual const char *GetTitle() const = 0;
};