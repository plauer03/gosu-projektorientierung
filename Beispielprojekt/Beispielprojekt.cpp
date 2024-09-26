#include <Gosu/Gosu.hpp>
#include <Gosu/AutoLink.hpp>

class GameWindow : public Gosu::Window
{
private:
    double window_width, window_height;  // Dynamische Variablen für Fenstergröße
    double x, y;                         // Position der Grafik
    double velocity_y = 0;               // Geschwindigkeit in Y-Richtung für das Springen
    bool on_ground = false;              // Flag, ob die Rakete den Boden berührt
    Gosu::Image image;
    Gosu::Image image2;                  // Bild für die Spitzhacken-Animation
    double ground_height;                // Höhe des Bodens
    bool facing_right = true;            // Flag, ob der Charakter nach rechts schaut
    bool left_mouse_pressed = false;     // Flag, ob die linke Maustaste gedrückt wird
    int animation_counter = 0;           // Zähler für den Bildwechsel bei gedrückter Maustaste
    Gosu::Image scar;               // Bild für die Waffe
    bool weapon_picked = false;     // Flag, ob die Waffe aufgehoben wurde
    Gosu::Font font;                // Schrift für den "aufheben E" Text
    bool near_weapon = false;       // Flag, ob der Spieler nahe der Waffe ist
    Gosu::Image skin3;              // Bild für den Charakter, wenn die Waffe aufgehoben wurde
    Gosu::Image explosion;
    Gosu::Image bg;
    bool weapon_dropped = false;
    Gosu::Image inv;
    Gosu::Image pickaxe;
    Gosu::Image health;
    Gosu::Image scarInv;
    bool in_frame = true;
    bool shoot = false;
    Gosu::Image bullet;
    double shoot_timer = 0;           // Timer für das regelmäßige Schießen
    Gosu::Sample sound1;

public:
    GameWindow()
        : Window(1280, 720), window_width(1280), window_height(720), x(320), y(560),
        image("skin.png"), image2("skin2.png"), skin3("skin3.png"), scar("scar.png"),
        ground_height(150), font(20), explosion("explosion.png"), bg("background.jpg"),
        inv("inventar.png"), pickaxe("pickaxe.png"), health("health.png"), scarInv("scar_inv.png"),
        bullet("bullet.png"), sound1("scarsound.mp3")
    {
        set_caption("Fortnite");
    }


    void update() override {
        // Schwerkraft hinzufügen, wenn nicht auf dem Boden
        if (!on_ground) {
            velocity_y += 1; // Gravitation
        }

        // Springen, wenn Leertaste gedrückt und auf dem Boden
        if (on_ground && Gosu::Input::down(Gosu::KB_SPACE)) {
            velocity_y = -15; // Sprungkraft
            on_ground = false;
        }

        // Bewegung links/rechts
        if (Gosu::Input::down(Gosu::KB_A)) {
            x -= 5; // Bewege nach links
            facing_right = false; // Charakter schaut nach links
        }
        if (Gosu::Input::down(Gosu::KB_D)) {
            x += 5; // Bewege nach rechts
            facing_right = true;  // Charakter schaut nach rechts
        }

        // Begrenze die Geschwindigkeit der Rakete nach unten
        if (velocity_y > 20) velocity_y = 20;

        // Aktualisiere die Y-Position durch die Geschwindigkeit
        y += velocity_y;

        // Verhindere, dass die Rakete den Boden durchdringt
        double ground_y = window_height - ground_height - image.height(); // Bodenhöhe korrekt setzen
        if (y >= ground_y) {
            y = ground_y;
            velocity_y = 0;
            on_ground = true;
        }
        else {
            on_ground = false; // Setze das Flag auf false, wenn die Rakete in der Luft ist
        }

        // Überprüfe, ob die linke Maustaste gedrückt ist
        if (Gosu::Input::down(Gosu::MS_LEFT)) {
            left_mouse_pressed = true;
            if (weapon_picked) {
                shoot = true;
            }
            else {
                animation_counter++; // Zähler für die Animation
            }
        }
        else {
            left_mouse_pressed = false;
            shoot = false;
            animation_counter = 0; // Zähler zurücksetzen, wenn die Maustaste losgelassen wird
        }


        // Kollisionserkennung zwischen Spieler und Waffe
        if (!weapon_picked && x + image.width() > 600 && x < 600 + scar.width() && y + image.height() >= window_height - ground_height - scar.height()) {
            near_weapon = true;  // Spieler ist in der Nähe der Waffe
        }
        else {
            near_weapon = false;
        }

        // Waffe aufheben, wenn Spieler in der Nähe und E drückt
        if (near_weapon && Gosu::Input::down(Gosu::KB_E)) {
            weapon_picked = true;
        }

        if (x <= window_width+300 && x >= 0-300) {
            in_frame = true;
        }
        else {
            in_frame = false;
            x = window_width / 2 - 100;
            y = 100;
        }
    }

    void draw() override {
        
        bg.draw(0,0, 1);
        pickaxe.draw(800, 600, 1);
        inv.draw(900, 600, 1);
        health.draw(50, 600, 1);

        // Zeichne die Animation: Wechsle zwischen image und image2
        Gosu::Image* current_image = &image; // Zeige das Standardbild an

        if (left_mouse_pressed) {
            // Wechsle zwischen image und image2 bei jedem "frame" der Animation
            if (animation_counter % 20 < 10) {
                current_image = &image2; // Wechsle zu skin2
            }
            else {
                current_image = &image;  // Wechsle zurück zu skin
            }
        }
        if (weapon_picked) {
            current_image = &skin3; // Bild des Charakters mit Waffe
            scarInv.draw(900, 600, 1);
        }
        // Zeichne den Charakter, je nachdem, ob er nach links oder rechts schaut
        if (facing_right) {
            // Normales Zeichnen (nach rechts)
            current_image->draw_rot(x + current_image->width() / 2, y + current_image->height() / 2, 1, 0, 0.5, 0.5, -1, 1);
            
        }
        else {
            // Spiegeln des Bildes nach links
            current_image->draw(x, y, 1);
        }

        // Zeichne die Waffe, wenn sie nicht aufgehoben wurde
        if (!weapon_picked && !weapon_dropped) {
            scar.draw(600, window_height - ground_height - scar.height(), 1);
        }

        // Zeichne den "aufheben E" Text, wenn der Spieler in der Nähe der Waffe ist
        if (near_weapon && !weapon_picked) {
            font.draw_text("Aufheben (E)", 600, window_height - ground_height - scar.height() - 30, 1, 1.0, 1.0, Gosu::Color::WHITE);
        }

        if (shoot) {
            if (facing_right) {
                graphics().draw_line(x+140, y+51, Gosu::Color::RED, x+1280, y+60, Gosu::Color::GRAY, 1.0);
                bullet.draw(1240, y + 48, 1);
                explosion.draw(1250, y + 25, 1);
            }
            else {
                graphics().draw_line(x+5, y+51, Gosu::Color::RED, x-1280, y+60, Gosu::Color::GRAY, 1.0);
                bullet.draw(20, y + 48,1);
                explosion.draw(-20, y + 25, 1);
                
            }
        }
    }
};

// C++ Hauptprogramm
int main()
{
    GameWindow window;
    window.show();
}
