#ifndef UICONTROLLER_H_
#define UICONTROLLER_H_

#include<CEGUI/RendererModules/Direct3D9/Renderer.h>
#include<CEGUI/CEGUI.h>
#include<d3d9.h>
#include<d3dx9.h>


class UIController
{

	CEGUI::DefaultResourceProvider* m_rp; // something related to file
	//CEGUI::WindowManager& m_winMgr; //window manager
	CEGUI::DefaultWindow* m_root; // root window

	// paritcle type choice, distance or time
	CEGUI::Combobox* m_particletypeCombobox; 
	CEGUI::ListboxTextItem* m_particletype_timerelease;
	CEGUI::ListboxTextItem* m_particletype_distancerelease;

	//particle life time 
	CEGUI::Window* m_lifetimelabel;
	CEGUI::Editbox* m_lifetimeeditbox;

	//start render or stoprender
	CEGUI::ButtonBase* m_startbutton;
	CEGUI::ButtonBase* m_stopbutton;



public:
	UIController();
	void Init(LPDIRECT3DDEVICE9 pd3dDevice);
	void Layout();
	void Render();
	static void InjectInputs(HWND hwnd,UINT message,WPARAM wParam,LPARAM lParam);

	void Release();
	~UIController();

private:
	static UINT virtualkey2scancode(WPARAM wParam, LPARAM lParam);

};








#endif