// Librería para la comunicación serie con el Arduino
import processing.serial.*;

// Librería para la emisión de sonidos
import processing.sound.*;

int tam_pixel = 5, direccion = 1, tam_grid  = tam_pixel * 7 + 5, puntuacion = 0, umbralPiezo = 6, control = 0;
boolean comprobar = false;
PFont f;

Jugador jugador;
ArrayList enemigos = new ArrayList();
ArrayList disparos = new ArrayList();

// Definición para la incorporación del puerto serie con Arduino
Serial Puerto;
String val;

// Definición de las variables para cargar las imágenes del juego
PImage img_jugador, img_enemigo;

// Definición de la variable para el fichero de sonido
SoundFile explosion, fin, laser;

void setup() 
{
    // Configuración del puerto serie con Arduino
    String portName = Serial.list()[0];
    Puerto = new Serial(this, portName, 9600);
    
    // Tamaño de la pantalla
    size(800, 550);
    
    // Inicialización de las clases
    jugador = new Jugador();
    crearEnemigos();
    
    img_jugador = loadImage("jugador.png");
    img_enemigo = loadImage("enemigo.png");  
    
    explosion = new SoundFile(this, "Explosion.wav");
    laser = new SoundFile(this, "laser.wav");
    fin = new SoundFile(this, "Fin.wav");
}

void draw() 
{
    if(Puerto.available() > 0) 
    {
        background(loadImage("fondo.jpg"));
        
        if(enemigos.size() == 0)
        {
          controlLetras(2);

          if(fin.isPlaying() == false)
          {
              crearEnemigos();
              puntuacion = 0;
              control = 0;
          }
        }
        else
        {
          controlLetras(1);
        }
    
        jugador.draw();
    
        for(int i = 0; i < disparos.size(); i++) 
        {
            ArmaJugador bullet = (ArmaJugador) disparos.get(i);
            
            // Si el laser del jugador llega a la coordenada de y determinada se eliminará liberando recursos
            if(bullet.coord_y < 40) 
            {
              disparos.remove(i);
            }
            
            bullet.draw();
        }
    
        for(int i = 0; i < enemigos.size(); i++) 
        {
            Enemigo enemy = (Enemigo) enemigos.get(i);
            
            if (enemy.fueraRango() == true) 
            {
                direccion *= (-1);
                comprobar = true;
                break;
            }
           
        }
    
        for (int i = 0; i < enemigos.size(); i++) 
        {
            
            Enemigo enemy = (Enemigo) enemigos.get(i);
            
            if (enemy.alive() == false) 
            {
                enemigos.remove(i);
            } 
            else 
            {
                enemy.draw();
            }
        }
    
        comprobar = false;
    }
}

void controlLetras(int i) 
{
    f = createFont("Arial", 30, true);
    fill(255);
    textFont(f);
    
    // Mostramos la puntuación
    if(i == 1)
    {
        text("Puntuacion: " + String.valueOf(puntuacion), 300, 50);
    }
    
    // Mostramos el fin de juego
    else if(i == 2)
    {
        if(fin.isPlaying() == false && control != 1)
        {
            control = 1;
            fin.play();
        }
        
        stroke(0);
        fill(color(random(255), random(255), random(255)));
        f = createFont("Arial", 30 + random(10), true);
        textFont(f);
        text("Fin del juego", 300, 50);
    }
}

void crearEnemigos() 
{
    for (int i = 0; i < 1; i++) 
    {
        for (int j = 0; j < 1; j++) 
        {
            enemigos.add(new Enemigo(i * tam_grid, j * tam_grid + 70));
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
        image(img_jugador, xpos, ypos, tam_grid + 10, tam_grid + 10);   
    }

    Jugador() 
    {
        x = width/tam_grid/2;
        y = height - (10 * tam_pixel);
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
        
        val = Puerto.readStringUntil('\n').trim();
        
        if((val != ""  && Integer.parseInt(val) > umbralPiezo) || (keyPressed && key== ENTER))
        {
          disparos.add(new ArmaJugador(x, y));
          canShoot = false;
          shootdelay = 0;
          if(!laser.isPlaying())
          {
            laser.play();
          }
        }
      
        shootdelay++;
        
        if (shootdelay >= 50) 
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
        image(img_enemigo, xpos, ypos, tam_grid, tam_grid);   
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
            x += direccion * tam_grid;
        }
        
        if (comprobar == true) 
        {
            y += tam_grid / 2;
        }
    }

    // Función que permite quitar la vida a los enemigos
    boolean alive() 
    {
        for (int i = 0; i < disparos.size(); i++) 
        {
            ArmaJugador bullet = (ArmaJugador) disparos.get(i);
            
            if (bullet.coord_x > x && bullet.coord_x < x + 7 * tam_pixel + 5 && bullet.coord_y > y && bullet.coord_y < y + 5 * tam_pixel) 
            {
                disparos.remove(i);
                
                vida--;
                
                if (vida == 0) 
                {
                    stroke(0);
                    explosion.play();
                    image(loadImage("explosion.png"), x, y, tam_grid * 2, tam_grid * 2); 
                    puntuacion += 50;
                    return false;
                }
                
                break;
            }
        }

        return true;
    }

    boolean fueraRango() 
    {
        return x + (direccion * tam_grid) < 0 || x + (direccion*tam_grid) > width - tam_grid;
    }
}

// Lanzamiento del jugador
class ArmaJugador 
{
    int coord_x, coord_y;

    ArmaJugador(int xpos, int ypos) 
    {
        coord_x = xpos + (tam_grid / 2) - 4;
        coord_y = ypos;
    }

    void draw() 
    {
        stroke(0);
        fill(1, 255, 0);
        rect(coord_x + 7, coord_y - 14, tam_pixel/2, tam_pixel * 3);
        coord_y -= tam_pixel * 2;
    }
}
