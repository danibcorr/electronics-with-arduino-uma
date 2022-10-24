// Librería para la comunicación serie con el Arduino
import processing.serial.*;

int pixelsize = 5, direction = 1, gridsize  = pixelsize * 7 + 5, puntuacion = 0;
boolean incy = false;
PFont f;

Jugador jugador;
ArrayList enemigos = new ArrayList();
ArrayList disparos = new ArrayList();

// Definición para la incorporación del puerto serie con Arduino
Serial Puerto;
String val;

PImage img_jugador, img_enemigo;

// Se dispara con el ctrl

void setup() 
{
    // Configuración del puerto serie con Arduino
    String portName = Serial.list()[1];
    Puerto = new Serial(this, portName, 9600);
    
    noStroke();
    
    // Tamaño de la pantalla
    size(800, 550);
    
    // Inicialización de las clases
    jugador = new Jugador();
    crearEnemigos();
    
    img_jugador = loadImage("jugador.png");
    img_enemigo = loadImage("enemigo.png");

    f = createFont("Arial", 36, true);
}

void draw() 
{
  if(Puerto.available() > 0) 
  {
    background(loadImage("fondo.png"));
    
    if (enemigos.size() == 0)
    {
      finJuego();
    }
    else
    {
      dibujarPuntuacion();
    }

    jugador.draw();

    for(int i = 0; i < disparos.size(); i++) 
    {
        ArmaJugador bullet = (ArmaJugador) disparos.get(i);
        bullet.draw();
    }

    for(int i = 0; i < enemigos.size(); i++) 
    {
        
        Enemigo enemy = (Enemigo) enemigos.get(i);
        
        if (enemy.outside() == true) 
        {
            direction *= (-1);
            incy = true;
            break;
        }
    }

    for (int i = 0; i < enemigos.size(); i++) 
    {
        
      Enemigo enemy = (Enemigo) enemigos.get(i);
        
        if (!enemy.alive()) 
        {
            enemigos.remove(i);
        } 
        else 
        {
            enemy.draw();
        }
    }

    incy = false;
  }
}

void dibujarPuntuacion() 
{
    fill(255);
    textFont(f);
    text("Puntuacion: " + String.valueOf(puntuacion), 300, 50);
}

void finJuego() 
{
    fill(255);
    textFont(f);
    text("Fin del juego", 300, 50);
}

void crearEnemigos() 
{
    for (int i = 0; i < 10; i++) 
    {
        for (int j = 0; j < 3; j++) 
        {
            enemigos.add(new Enemigo(i*gridsize, j*gridsize + 70));
        }
    }
}

class Jugador
{
    boolean canShoot = true;
    int shootdelay = 0, x, y;;
    
    void draw() 
    {
        updateObj();
        drawSprite(x, y);
    }

    void drawSprite(int xpos, int ypos) 
    {
        image(img_jugador, xpos, ypos, gridsize + 10, gridsize + 10);   
    }

    Jugador() 
    {
        x = width/gridsize/2;
        y = height - (10 * pixelsize);
    }
    
    void updateObj() 
    {
        if (keyPressed && keyCode == LEFT) 
        {
            x -= 5;
        }
        else if (keyPressed && keyCode == RIGHT) 
        {
            x += 5;
        }
        
        val = Puerto.readStringUntil('\n').trim();
        
        if(val != ""  && Integer.parseInt(val) > 6)
        {
          disparos.add(new ArmaJugador(x, y));
          canShoot = false;
          shootdelay = 0;
        }
      
        shootdelay++;
        
        if (shootdelay >= 20) 
        {
            canShoot = true;
        }
    }
}

class Enemigo
{
    int vida = 3, x, y;
        
    void draw() 
    {
        updateObj();
        drawSprite(x, y);
    }

    void drawSprite(int xpos, int ypos) 
    {
        stroke(0);
        image(img_enemigo, xpos, ypos, gridsize, gridsize);   
    }
    
    // Constructor de los enemigos
    Enemigo(int xpos, int ypos) 
    {
        x = xpos;
        y = ypos;
    }
    
    // Actualización de la posición de los enemigos
    void updateObj() 
    {
        if (frameCount%30 == 0) 
        {
            x += direction * gridsize;
        }
        
        if (incy == true) 
        {
            y += gridsize / 2;
        }
    }

    // Función que permite quitar la vida a los enemigos
    boolean alive() 
    {
        for (int i = 0; i < disparos.size(); i++) 
        {
            ArmaJugador bullet = (ArmaJugador) disparos.get(i);
            
            if (bullet.coord_x > x && bullet.coord_x < x + 7 * pixelsize + 5 && bullet.coord_y > y && bullet.coord_y < y + 5 * pixelsize) 
            {
                disparos.remove(i);
                
                vida--;
                
                if (vida == 0) 
                {
                    stroke(0);
                    image(loadImage("explosion.png"), x, y, gridsize * 2, gridsize * 2); 
                    puntuacion += 50;
                    return false;
                }
                
                break;
            }
        }

        return true;
    }

    boolean outside() 
    {
        return x + (direction*gridsize) < 0 || x + (direction*gridsize) > width - gridsize;
    }
}

// Lanzamiento del jugador
class ArmaJugador 
{
    int coord_x, coord_y;

    ArmaJugador(int xpos, int ypos) 
    {
        coord_x = xpos + gridsize/2 - 4;
        coord_y = ypos;
    }

    void draw() 
    {
        stroke(0);
        fill(1, 255, 0);
        rect(coord_x + 7, coord_y - 14, pixelsize/2, pixelsize * 3);
        coord_y -= pixelsize * 2;
    }
}
