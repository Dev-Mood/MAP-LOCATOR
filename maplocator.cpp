#include <SFML/Graphics.hpp>
#include <curl/curl.h>
#include <iostream>
#include <string>
#include <vector>

using namespace sf;

// Function to download map tile from OpenStreetMap API
size_t write_data(void *ptr, size_t size, size_t nmemb, void *stream) {
    std::vector<char> *buffer = (std::vector<char>*)stream;
    size_t count = size * nmemb;
    buffer->insert(buffer->end(), (char*)ptr, (char*)ptr + count);
    return count;
}

void download_tile(int x, int y, int z, std::vector<char>& tileData) {
    CURL *curl;
    CURLcode res;

    std::string url = "https://a.tile.openstreetmap.org/" + std::to_string(z) + "/" + std::to_string(x) + "/" + std::to_string(y) + ".png";
    std::cout << "Downloading tile: " << url << std::endl;

    curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &tileData);
        res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);
    }
}

int main() {
    // Create window
    RenderWindow window(VideoMode(800, 600), "Map Locator");

    // Create sprite and texture for map tile
    Sprite mapSprite;
    Texture mapTexture;

    // Download map tile and create texture
    int tileX = 19157;
    int tileY = 24637;
    int tileZoom = 16;
    std::vector<char> tileData;
    download_tile(tileX, tileY, tileZoom, tileData);
    mapTexture.loadFromMemory(tileData.data(), tileData.size());
    mapSprite.setTexture(mapTexture);

    // Run main loop
    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed)
                window.close();
        }

        // Clear screen and draw map
        window.clear();
        window.draw(mapSprite);
        window.display();
    }

    return 0;
}

