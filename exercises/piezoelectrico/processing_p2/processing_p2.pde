// WARNING: If using headphones, please lower the audio for your health.

// This project is a modification of the following code: https://gist.github.com/ihavenonickname/5cc5b9b1d9b912f704061a241bc096ad

// Library for serial communication with Arduino
import processing.serial.*;

// Library for sound playback
import processing.sound.*;

int pixel_size = 5, direction = 1, grid_size = pixel_size * 7 + 5, score = 0, piezoThreshold = 2, control = 0;
boolean check = false;
PFont f;

Player player;
ArrayList enemies = new ArrayList();
ArrayList shots = new ArrayList();

// Definition for serial port incorporation with Arduino
Serial port;
String val;

// Definition of variables for loading game images
PImage player_img, enemy_img;

// Definition of the variable for the sound file
SoundFile explosion, gameOver, laser;

void setup() 
{
    // Serial port configuration with Arduino
    String portName = Serial.list()[0];
    port = new Serial(this, portName, 9600);
    
    // Screen size
    size(800, 550);
    
    // Class initialization
    player = new Player();
    createEnemies();
    
    player_img = loadImage("player.png");
    enemy_img = loadImage("enemy.png");  
    
    explosion = new SoundFile(this, "Explosion.wav");
    laser = new SoundFile(this, "laser.wav");
    gameOver = new SoundFile(this, "GameOver.wav");
}

void draw() 
{
    if(port.available() > 0) 
    {
        background(loadImage("background.jpg"));
        
        if(enemies.size() == 0)
        {
          controlText(2);

          if(gameOver.isPlaying() == false)
          {
              createEnemies();
              score = 0;
              control = 0;
          }
        }
        else
        {
          controlText(1);
        }
    
        player.draw();
    
        for(int i = 0; i < shots.size(); i++) 
        {
            PlayerWeapon bullet = (PlayerWeapon) shots.get(i);
            
            // If the player's laser reaches a certain y-coordinate, it will be removed to free up resources
            if(bullet.coord_y < 40) 
            {
              shots.remove(i);
            }
            
            bullet.draw();
        }
    
        for(int i = 0; i < enemies.size(); i++) 
        {
            Enemy enemy = (Enemy) enemies.get(i);
            
            if (enemy.outOfRange() == true) 
            {
                direction *= (-1);
                check = true;
                break;
            }
           
        }
    
        for (int i = 0; i < enemies.size(); i++) 
        {
            
            Enemy enemy = (Enemy) enemies.get(i);
            
            if (enemy.alive() == false) 
            {
                enemies.remove(i);
            } 
            else 
            {
                enemy.draw();
            }
        }
    
        check = false;
    }
}

void controlText(int i) 
{
    f = createFont("Arial", 30, true);
    fill(255);
    textFont(f);
    
    // Display the score
    if(i == 1)
    {
        text("Score: " + String.valueOf(score), 300, 50);
    }
    
    // Display the end of the game
    else if(i == 2)
    {
        if(gameOver.isPlaying() == false && control != 1)
        {
            control = 1;
            gameOver.play();
        }
        
        stroke(0);
        fill(color(random(255), random(255), random(255)));
        f = createFont("Arial", 30 + random(10), true);
        textFont(f);
        text("Game Over", 300, 50);
    }
}

void createEnemies() 
{
    for (int i = 0; i < 5; i++) 
    {
        for (int j = 0; j < 3; j++) 
        {
            enemies.add(new Enemy(i * grid_size, j * grid_size + 70));
        }
    }
}

class Player
{
    boolean canShoot = true;
    int shootDelay = 0, x, y;
    
    void draw() 
    {
        updateObj();
        drawSprite(x, y);
    }

    void drawSprite(int xpos, int ypos) 
    {
        image(player_img, xpos, ypos, grid_size + 10, grid_size + 10);   
    }

    Player() 
    {
        x = width/grid_size/2;
        y = height - (10 * pixel_size);
    }
    
    void updateObj() 
    {
        if(keyPressed && keyCode == LEFT) 
        {
            x -= 5;
        }
        else if(keyPressed && keyCode == RIGHT) 
        {
            x += 5;
        }
        
        val = port.readStringUntil('\n').trim();
        print(val);
        
        if((val != null  && Integer.parseInt(val) > piezoThreshold) || (keyPressed && key == ENTER))
        {
          shots.add(new PlayerWeapon(x, y));
          canShoot = false;
          shootDelay = 0;
          if(!laser.isPlaying())
          {
            laser.play();
          }
        }
      
        shootDelay++;
        
        if (shootDelay >= 50) 
        {
            canShoot = true;
        }
    }
}

class Enemy
{
    int health = 3, x, y;
        
    void draw() 
    {
        updateObj();
        drawSprite(x, y);
    }

    void drawSprite(int xpos, int ypos) 
    {
        stroke(0);
        image(enemy_img, xpos, ypos, grid_size, grid_size);   
    }
    
    // Constructor for the enemies
    Enemy(int xpos, int ypos) 
    {
        x = xpos;
        y = ypos;
    }
    
    // Update the position of the enemies
    void updateObj() 
    {
        if (frameCount % 30 == 0) 
        {
            x += direction * grid_size;
        }
        
        if (check == true) 
        {
            y += grid_size / 2;
        }
    }

    // Function that reduces the enemy's health
    boolean alive() 
    {
        for (int i = 0; i < shots.size(); i++) 
        {
            PlayerWeapon bullet = (PlayerWeapon) shots.get(i);
            
            if (bullet.coord_x > x && bullet.coord_x < x + 7 * pixel_size + 5 && bullet.coord_y > y && bullet.coord_y < y + 5 * pixel_size) 
            {
                shots.remove(i);
                
                health--;
                
                if (health == 0) 
                {
                    stroke(0);
                    explosion.play();
                    image(loadImage("explosion.png"), x, y, grid_size * 2, grid_size * 2); 
                    score += 50;
                    return false;
                }
                
                break;
            }
        }

        return true;
    }

    boolean outOfRange() 
    {
        return x + (direction * grid_size) < 0 || x + (direction * grid_size) > width - grid_size;
    }
}

// Player's weapon shot
class PlayerWeapon 
{
    int coord_x, coord_y;

    PlayerWeapon(int xpos, int ypos) 
    {
        coord_x = xpos + (grid_size / 2) - 4;
        coord_y = ypos;
    }

    void draw() 
    {
        stroke(0);
        fill(1, 255, 0);
        rect(coord_x + 7, coord_y - 14, pixel_size/2, pixel_size * 3);
        coord_y -= pixel_size * 2;
    }
}
