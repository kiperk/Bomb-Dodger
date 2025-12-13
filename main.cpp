#include <iostream>
#include <raylib.h>
#include <stdlib.h>
#include <vector>
#include <ctime>


class Game {
	public:
		int s_width = 720;
		int s_height = 720;
		Color blue = {0, 0, 139,255};
    	int screen = 500;  
    	Color r_color = {135, 60, 190, 255}; 
    	float timer = 0.0f;
    	bool timer_running = false; 

    void draw_timer() {
        if (timer_running) {
            // Format: MM:SS
            int minutes = (int)timer / 60;
            int seconds = (int)timer % 60;
            
            DrawText(TextFormat("TIME: %02d:%02d.%03d", minutes, seconds), 
                    550, 10, 20, GREEN);
        }
    }		

    void timer_update(){
    	if(timer_running){
    		timer += GetFrameTime();
    	}
    }

    void start_timer(){
    	timer_running = true;
    	timer = 0.0f;

    }

    void stop_timer(){
    	timer_running = false;
    }


    void death_screen(float survival_time){

    	// Semi-transparent overlay
        DrawRectangle(0, 0, s_width, s_height, {0, 0, 0, 180});
        
        // Death message
        DrawText("YOU DIED!", s_width/2 - MeasureText("YOU DIED!", 60)/2, 150, 60, RED);
        
        // Survival time
        int minutes = (int)survival_time / 60;
        int seconds = (int)survival_time % 60;
        int milliseconds = (int)(survival_time * 1000) % 1000;
        
        DrawText(TextFormat("Survived: %02d:%02d.%03d", minutes, seconds, milliseconds), 
                s_width/2 - MeasureText(TextFormat("Survived: %02d:%02d.%03d", minutes, seconds, milliseconds), 30)/2, 
                250, 30, WHITE);
        // Restart prompt
        DrawText("Press SPACE to Restart", 
                s_width/2 - MeasureText("Press SPACE to Restart", 30)/2, 
                400, 30, GREEN);

    }
    void reset_game() {
        timer_running = false;
        timer = 0.0f;
        
    }
};



class bombman{
	public:

			Vector2 bombman_pos;
			int r_bombman_posx;
			int r_bombman_posy;

			Texture2D bombman_r;
			Texture2D bombman_l;
			Texture2D bombman_u;
			Texture2D bombman_d;	


			Texture2D detect_bomber_tex;

	bombman(int screen){

		r_bombman_posx = rand() % screen;
		r_bombman_posy = rand() % screen;
		bombman_pos = { (float)r_bombman_posx, (float)r_bombman_posy };	

		bombman_r = LoadTexture("bombmanRIGHT.jpeg");
		std::cout << "4. Loaded DOWN" << std::endl;

		bombman_l = LoadTexture("bombmanLEFT.png");
		 std::cout << "4. Loaded LEFT" << std::endl;

		bombman_u = LoadTexture("bombmanUP.jpeg");
		 std::cout << "4. Loaded UP" << std::endl;


		bombman_d = LoadTexture("bombmanDOWN.jpeg");
		 std::cout << "4. Loaded DOWN" << std::endl;



		detect_bomber_tex = bombman_r;
		  std::cout << "4. Loaded ALL GOOD!" << std::endl;

	}		

	void draw_bomber(){	
			DrawTexture(detect_bomber_tex, bombman_pos.x, bombman_pos.y , WHITE); 
			
	}	
	Rectangle bomber_rec(){
    return Rectangle{bombman_pos.x, bombman_pos.y, 30, 30};

	}


};


class bombs {
	public:

		Texture2D bomb;

		#define max_bombs  10
		Vector2 position[max_bombs];
		Vector2 velocity[max_bombs];
		int bomb_count = max_bombs;

		int r_bomb_posx;
		int r_bomb_posy; 

	bombs(int screen){
			bomb  = LoadTexture("bomb.png");
			
			for(int i = 0 ; i < bomb_count ; i++){
			
			position[i] = {  (float) (rand() % screen) , (float) (rand() % screen) };

			velocity[i] = {  (float) (rand() % 16 - 5), (float) (rand() % 16 - 5)  };
	}		
	}	


	void draw_bombs(){
		for(int i = 0; i < bomb_count; i++){
			DrawTexture(bomb , position[i].x , position[i].y ,WHITE);
		}
		
			
	}	
	
	void update(){

		for(int i = 0 ;i < bomb_count; i++){

			position[i].x += velocity[i].x;
			position[i].y += velocity[i].y;

			if(position[i].x > 670 || position[i].x < 0){
					 velocity[i].x *= -1;
			}
			if(position[i].y > 720 || position[i].y < 0){
					 velocity[i].y *= -1;
			}
	}		
	}


	bool check_collision(Rectangle player_rect) {

    for (int i = 0; i < bomb_count; i++) {
        Rectangle bomb_rect = {position[i].x, position[i].y, 30, 30};
        if (CheckCollisionRecs(player_rect, bomb_rect)) {
            
            position[i] = {
                (float)(rand() % 500), 
                (float)(rand() % 500)
            };
            return true;  
        }
    }
    return false;  
}
};


int main() {


	srand(time(NULL));
	Game game;

	InitWindow ( game.s_width , game.s_height,"Bomb-dodger");
	SetTargetFPS(60);	
			
	bool show_death_screen = false;
	bool bombman_alive = true;
	
	bombman bombman(game.screen); 
	bombs bomb_manager(game.screen);
	
	game.start_timer();
  	
while (WindowShouldClose() == false){
        
	 // Handle restart key (R)
    if (IsKeyPressed(KEY_SPACE) && show_death_screen) {
            // Reset everything
            bombman_alive = true;
            show_death_screen = false;
            game.reset_game();
            game.start_timer();
            
            // Reset player position
            bombman.bombman_pos = { 
                (float)(rand() % game.screen), 
                (float)(rand() % game.screen) 
            };
	}
	if (bombman_alive){

	if (IsKeyDown(KEY_D)){
			 bombman.bombman_pos.x += 7.0f; 
			 bombman.detect_bomber_tex = bombman.bombman_r; 
			 if (bombman.bombman_pos.x > game.s_width - 70 ){
					bombman.bombman_pos.x  = game.s_width- 70;
			}
		}

	if (IsKeyDown(KEY_A)){
  			bombman.bombman_pos.x -= 7.0f; 
			bombman.detect_bomber_tex = bombman.bombman_l;\
			if (bombman.bombman_pos.x < 0 ){
					bombman.bombman_pos.x = 0;
			} 
		}

	if (IsKeyDown(KEY_W)){
		  	 bombman.bombman_pos.y -= 7.0f; 
			 bombman.detect_bomber_tex = bombman.bombman_u;
			 if (bombman.bombman_pos.y < 0 ){
					bombman.bombman_pos.y = 0;
			} 
		}

	if (IsKeyDown(KEY_S)){
			 bombman.bombman_pos.y += 7.0f; 
			 bombman.detect_bomber_tex = bombman.bombman_d; 
			 if (bombman.bombman_pos.y > game.s_height - 70){
					bombman.bombman_pos.y = game.s_height - 70;
			}
		}
	 

	if  (bomb_manager.check_collision(bombman.bomber_rec())  ){
				bombman_alive = false; 
				show_death_screen =  true;
				game.stop_timer();
	      		
	}
	}

	bomb_manager.update();
	game.timer_update();

    BeginDrawing();
	ClearBackground(game.blue);
	DrawFPS( 10,  10);

	game.draw_timer();
	bomb_manager.draw_bombs();
	bombman.draw_bomber();
	
    if (show_death_screen){
 	 	game.death_screen(game.timer);// then we a activate the game.show death screen function 	

	}										
	EndDrawing();     		
    
    		
	
}  
UnloadTexture(bombman.bombman_r);
UnloadTexture(bombman.bombman_l);
UnloadTexture(bombman.bombman_u);
UnloadTexture(bombman.bombman_d);
UnloadTexture(bomb_manager.bomb);										
 
CloseWindow();
return 0;


}	
	
 	 
    

