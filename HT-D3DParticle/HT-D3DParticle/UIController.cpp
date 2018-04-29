#include"UIController.h"

#define SAFE_DELETE(p) {if(p){ delete (p); (p) = NULL;}}

UIController::UIController()
{

}

void UIController::Init(LPDIRECT3DDEVICE9 pd3dDevice)
{
     CEGUI::Direct3D9Renderer& m_myRenderer = CEGUI::Direct3D9Renderer::bootstrapSystem(pd3dDevice);

	//cegui initialize
    m_rp = static_cast<CEGUI::DefaultResourceProvider*>
		(CEGUI::System::getSingleton().getResourceProvider());

	m_rp->setResourceGroupDirectory("schemes", "datafiles/schemes/");
	m_rp->setResourceGroupDirectory("imagesets", "datafiles/imagesets/");
	m_rp->setResourceGroupDirectory("fonts", "datafiles/fonts/");
	m_rp->setResourceGroupDirectory("layouts", "datafiles/layouts/");
	m_rp->setResourceGroupDirectory("looknfeels", "datafiles/looknfeel/");
	m_rp->setResourceGroupDirectory("lua_scripts", "datafiles/lua_scripts/");

	CEGUI::ImageManager::setImagesetDefaultResourceGroup("imagesets");
	CEGUI::Font::setDefaultResourceGroup("fonts");
	CEGUI::Scheme::setDefaultResourceGroup("schemes");
	CEGUI::WidgetLookManager::setDefaultResourceGroup("looknfeels");
	CEGUI::WindowManager::setDefaultResourceGroup("layouts");
	CEGUI::ScriptModule::setDefaultResourceGroup("lua_scripts");


	CEGUI::SchemeManager::getSingleton().createFromFile( "TaharezLook.scheme" );
	CEGUI::SchemeManager::getSingleton().createFromFile("AlfiskoSkin.scheme");
	CEGUI::SchemeManager::getSingleton().createFromFile("WindowsLook.scheme");
	CEGUI::SchemeManager::getSingleton().createFromFile("VanillaSkin.scheme");
	CEGUI::SchemeManager::getSingleton().createFromFile("OgreTray.scheme");





	CEGUI::WindowManager& winMgr = CEGUI::WindowManager::getSingleton();

	m_root = (CEGUI::DefaultWindow*)winMgr.createWindow("DefaultWindow","root");

	CEGUI::System::getSingleton().getDefaultGUIContext().setRootWindow(m_root);

	//////////////////////////////////////////////////////////////////////////
	//UI Initialize

	//ParticleType Combobox
	m_particletypeCombobox = static_cast<CEGUI::Combobox*>(winMgr.createWindow("Vanilla/Combobox","ParticleTypeCombobox"));
	m_particletype_timerelease = new CEGUI::ListboxTextItem("TimeRelease");
	m_particletype_distancerelease = new CEGUI::ListboxTextItem("DistanceRelease");
	m_particletypeCombobox->addItem(m_particletype_timerelease);
	m_particletypeCombobox->addItem(m_particletype_distancerelease);
	m_particletypeCombobox->setItemSelectState(m_particletype_timerelease,true);



	//LifeTime Edit
	m_lifetimelabel = winMgr.createWindow("TaharezLook/Label","Label");
	m_lifetimelabel->setText("LifeTime");

	m_lifetimeeditbox = static_cast<CEGUI::Editbox*>(winMgr.createWindow("Vanilla/Editbox", "LifeTimeEditBox"));

	//Buttons
	m_startbutton = static_cast<CEGUI::ButtonBase *>(winMgr.createWindow("TaharezLook/Button", "StartButton"));
	m_stopbutton = static_cast<CEGUI::ButtonBase *>(winMgr.createWindow("TaharezLook/Button", "StopButton"));
}

bool handleSkinSelectionAccepted(const CEGUI::EventArgs& args)
{
	const CEGUI::WindowEventArgs& winEventArgs = static_cast<const CEGUI::WindowEventArgs&>(args);
	CEGUI::Combobox* skinSelectionCombobox = static_cast<CEGUI::Combobox*>(winEventArgs.window);

	CEGUI::ListboxItem* selectedItem = skinSelectionCombobox->getSelectedItem();


	return false;
}

void UIController::Layout()
{

	m_particletypeCombobox->setPosition(CEGUI::UVector2(cegui_reldim(0.80f), cegui_reldim( 0.2f)));
	m_particletypeCombobox->setSize(CEGUI::USize(cegui_reldim(0.20f), cegui_reldim(0.15f)));
	m_particletypeCombobox->setReadOnly(true);
	m_particletypeCombobox->setSortingEnabled(false);
	m_root->addChild(m_particletypeCombobox);

	m_lifetimelabel->setPosition(CEGUI::UVector2(cegui_reldim(0.80f), cegui_reldim( 0.3f)));
	m_lifetimelabel->setSize(CEGUI::USize(cegui_reldim(0.10f), cegui_reldim(0.04f)));

	m_root->addChild(m_lifetimelabel);

	m_lifetimeeditbox->setPosition(CEGUI::UVector2(cegui_reldim(0.9f), cegui_reldim( 0.3f)));
	m_lifetimeeditbox->setSize(CEGUI::USize(cegui_reldim(0.10f), cegui_reldim(0.04f)));
	m_lifetimeeditbox->setText("");
	m_lifetimeeditbox->setReadOnly(false);

	m_root->addChild(m_lifetimeeditbox);

	/*editbox->setPosition(CEGUI::UVector2(cegui_reldim(0.3f), cegui_reldim( 0.3f)));
	editbox->setSize(CEGUI::USize(cegui_reldim(0.25f), cegui_reldim(0.04f)));
	editbox->setText("");
	editbox->setReadOnly(false);*/
}

void UIController::Render()
{
	CEGUI::System::getSingleton().renderAllGUIContexts();
}

void UIController::InjectInputs(HWND hwnd,UINT message,WPARAM wParam,LPARAM lParam)
{

	switch (message)
	{
	case WM_MOUSEMOVE: 
		CEGUI::System::getSingleton().getDefaultGUIContext().injectMousePosition((float)(LOWORD(lParam)), 
			(float)(HIWORD(lParam))); 
		break; 
	case WM_LBUTTONDOWN: 
		CEGUI::System::getSingleton().getDefaultGUIContext().injectMouseButtonDown(CEGUI::LeftButton); 
		break; 
	case WM_LBUTTONUP: 
		CEGUI::System::getSingleton().getDefaultGUIContext().injectMouseButtonUp(CEGUI::LeftButton); 
		break; 
	case WM_RBUTTONDOWN: 
		CEGUI::System::getSingleton().getDefaultGUIContext().injectMouseButtonDown(CEGUI::RightButton); 
		break; 
	case WM_RBUTTONUP: 
		CEGUI::System::getSingleton().getDefaultGUIContext().injectMouseButtonUp(CEGUI::RightButton); 
		break;
	case WM_MBUTTONDOWN: 
		CEGUI::System::getSingleton().getDefaultGUIContext().injectMouseButtonDown(CEGUI::MiddleButton); 
		break;
	case WM_MBUTTONUP: 
		CEGUI::System::getSingleton().getDefaultGUIContext().injectMouseButtonUp(CEGUI::MiddleButton); 
		break; 
	case WM_MOUSEWHEEL: 
		CEGUI::System::getSingleton().getDefaultGUIContext().injectMouseWheelChange(((short)HIWORD(wParam)) / (120.0)); 
		break;
	case WM_KEYDOWN:
		CEGUI::System::getSingleton().getDefaultGUIContext().injectKeyDown(CEGUI::Key::Scan(virtualkey2scancode(wParam,lParam)));
		break;
	case WM_KEYUP:
		CEGUI::System::getSingleton().getDefaultGUIContext().injectKeyUp(CEGUI::Key::Scan(virtualkey2scancode(wParam,lParam)));
		break;
	case WM_CHAR:
		CEGUI::System::getSingleton().getDefaultGUIContext().injectChar( static_cast<CEGUI::utf32>(wParam) );
		break;
	default:
		break;
	}
}

UINT UIController::virtualkey2scancode(WPARAM wParam, LPARAM lParam)
	{
		if(HIWORD(lParam) & 0x0F00)
		{
			UINT scancode = MapVirtualKey(wParam, 0); 
			return scancode | 0x80;
		}
		else
		{
			return HIWORD(lParam) & 0x00FF;
		}
	}

UIController::~UIController()
{


/*	SAFE_DELETE(m_rp); // something related to file

	SAFE_DELETE(m_root); // root window

	// paritcle type choice, distance or time
	SAFE_DELETE(m_particletypeCombobox); 
	SAFE_DELETE(m_particletype_timerelease);
	SAFE_DELETE(m_particletype_distancerelease);

	//particle life time 
	SAFE_DELETE(m_lifetimelabel);

	SAFE_DELETE(m_lifetimeeditbox);

	//start render or stoprender
	SAFE_DELETE(m_startbutton);
	SAFE_DELETE(m_stopbutton);*/
}

void UIController::Release()
{
	//CEGUI::SchemeManager::getSingleton().destroy("TaharezLook.scheme");
	CEGUI::Direct3D9Renderer::destroySystem();

/*	SAFE_DELETE(m_rp); // something related to file

	SAFE_DELETE(m_root); // root window

	// paritcle type choice, distance or time
	SAFE_DELETE(m_particletypeCombobox); 
	SAFE_DELETE(m_particletype_timerelease);
	SAFE_DELETE(m_particletype_distancerelease);

	//particle life time 
	SAFE_DELETE(m_lifetimelabel);

	SAFE_DELETE(m_lifetimeeditbox);

	//start render or stoprender
	SAFE_DELETE(m_startbutton);
	SAFE_DELETE(m_stopbutton);*/
}

