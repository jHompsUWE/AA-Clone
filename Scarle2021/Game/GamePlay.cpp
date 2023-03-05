#include "pch.h"
#include "GamePlay.h"

#include "UIPanel.h"

GamePlay::GamePlay()
= default;

GamePlay::~GamePlay()
{
    delete ui_frame;
    delete window_one;
}

bool GamePlay::init()
{
    // window init
    window_one = new UIWindow(Vector2(*DataManager::GetRES().first*0.5,*DataManager::GetRES()
        .second*0.5),DataManager::GetD3DDevice(),"","Window",Vector2(0.5,0.5));

    main_panel = new UIPanel(Vector2(0,30),DataManager::GetD3DDevice(),"UIPanel",Vector2(1,1)); 
    
    // ui frame init
    ui_frame = new ImageGO2D("UIFrame",DataManager::GetD3DDevice());
    ui_frame->SetOrigin(Vector2(0,0));
    ui_frame->SetScale(Vector2(1,1));
    
    // Mouse
    mouse_screen_pos.z = 0;
    mouse_world_pos = std::make_shared<Vector3>(0, 0, 0);

    // Building System
    building_system = std::make_unique<BuildingSystem>(mouse_world_pos, DataManager::GetD3DDevice());

    adv_man = std::make_unique<AdvisorManager>();
    adv_man->init();

    economy_manager = new EconomyManager();

    return true;
}

void GamePlay::Update(GameData* game_data)
{   
    // mouse pos
    auto mouse_pos = Vector2(game_data->mouse_state.x, game_data->mouse_state.y);

    //update window
    window_one->update(game_data,mouse_pos);
    
    //updates panel
    main_panel->update(game_data,mouse_pos);

    adv_man->Update(game_data);

    economy_manager->UpdateCurrency();
}

void GamePlay::ScaledUpdate(GameData* game_data, float& scaled_dt)
{
}

void GamePlay::LateUpdate(GameData* game_data)
{
    // Update mouse position
    mouse_screen_pos.x = game_data->mouse_state.x;
    mouse_screen_pos.y = game_data->mouse_state.y;

    // Building System
    building_system->Tick(game_data);
}

void GamePlay::GetEvents(std::list<AfterlifeEvent>& event_list)
{
    // BuildingSystem
    building_system->GetEvents(event_list);

    for (auto& event : event_list)
    {
        switch (event)
        {
        case none:
            std::cout << "soooos" << std::endl;
            break;

        case window_1_green:
            window_one_open = !window_one_open;

            break;

        case enter_main_menu:
            break;

        case input_left:
            break;

        case game_resized:
            ResizeUI();
            break;

        case input_right:
            DataManager::GetGD()->current_game_state = gs_game_over;
            break;
        }
    }
}

void GamePlay::Render2D(DrawData2D* draw_data2D)
{
    /*
    ui_frame->Draw(draw_data2D);

    //renders panel
    main_panel->render(draw_data2D);
    
    // checks if window is open to render
    if (window_one_open)
    {
        //render window
        window_one->render(draw_data2D);
    }
    */
}

void GamePlay::Render3D(DrawData* draw_data)
{
    UpdateMousePos(draw_data);
    
    // Building System
    building_system->Render3D(draw_data);
}

void GamePlay::UpdateMousePos(DrawData* draw_data)
{
    // Translate screen pos to world pos
    // TODO: update the screen width and height
    Vector3 temp_world_pos = XMVector3Unproject(mouse_screen_pos, 0, 0, 1080, 720,
        draw_data->main_camera->GetNearZ(),
        draw_data->main_camera->GetFarZ(),
        draw_data->main_camera->GetProj(),
        draw_data->main_camera->GetView(),
        draw_data->main_camera->GetWorldMatrix());

    *mouse_world_pos = Raycast::GetPosOnY(0, draw_data->main_camera->GetDirection(), temp_world_pos);

    // Offset by camera movement
    mouse_world_pos->x += draw_data->main_camera->GetTarget().x;
    mouse_world_pos->z += draw_data->main_camera->GetTarget().z;

    // Floor mouse_world_pos to tilemap grid (each tile is 1x1 unit)
    *mouse_world_pos = Vector3(std::floor(mouse_world_pos->x), 0, std::floor(mouse_world_pos->z));
}

void GamePlay::ResizeUI()
{
    ui_frame->ReSize(DataManager::GetRES().first, DataManager::GetRES().second);
    window_one->reSize(DataManager::GetRES());
    main_panel->reSize(DataManager::GetRES());
}

