#include <Gosu/Gosu.hpp>
#include <Gosu/AutoLink.hpp>

class GameWindow : public Gosu::Window
{
private:
    double x, y;               // Position der Grafik
    double velocity_y = 0;      // Geschwindigkeit in Y-Richtung für das Springen
    bool on_ground = false;     // Flag, ob die Rakete den Boden berührt
    Gosu::Image image;

public:
    GameWindow()
        : Window(800, 600), x(320), y(560), image("rakete.png")
    {
        set_caption("Gosu Tutorial mit Git");
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
        if (Gosu::Input::down(Gosu::KB_A)) x -= 5; // Bewege nach links
        if (Gosu::Input::down(Gosu::KB_D)) x += 5; // Bewege nach rechts

        // Begrenze die Geschwindigkeit der Rakete nach unten
        if (velocity_y > 20) velocity_y = 20;

        // Aktualisiere die Y-Position durch die Geschwindigkeit
        y += velocity_y;

        // Verhindere, dass die Rakete den Boden durchdringt
        if (y >= 560) { // Bodenhöhe ist 560 (20px über dem unteren Rand)
            y = 560;
            velocity_y = 0;
            on_ground = true;
        }
    }

    void draw() override {
        // Zeichne den Himmel (helles Blau)
        Gosu::Graphics::flush(); // Erstelle einen Hintergrund
        graphics().draw_quad(
            0, 0, Gosu::Color::CYAN,   // Oben links
            800, 0, Gosu::Color::CYAN, // Oben rechts
            0, 580, Gosu::Color::WHITE, // Unten links
            800, 580, Gosu::Color::WHITE, // Unten rechts
            0.0
        );

        // Zeichne die Rakete an der aktuellen Position
        image.draw(x, y, 1);

        // Zeichne die grüne Fläche (Boden)
        graphics().draw_quad(
            0, 580, Gosu::Color::GREEN,    // Oben links
            800, 580, Gosu::Color::GREEN,  // Oben rechts
            0, 600, Gosu::Color::GREEN,    // Unten links
            800, 600, Gosu::Color::GREEN,  // Unten rechts
            0.0
        );
    }
};

// C++ Hauptprogramm
int main()
{
    GameWindow window;
    window.show();
}
