#include <iostream>
#include <string>
#include <cstdlib>
#include <curl/curl.h>
#include "modules/json.hpp"

// Color variables
const std::string RESET = "\033[0m";
const std::string BLACK = "\033[30m";
const std::string RED = "\033[31m";
const std::string GREEN = "\033[32m";
const std::string YELLOW = "\033[33m";
const std::string BLUE = "\033[34m";
const std::string MAGENTA = "\033[35m";
const std::string CYAN = "\033[36m";
const std::string WHITE = "\033[37m";

// namespace for JSON
using json = nlohmann::json;

// CURL callback
size_t WriteCallback(void* ptr, size_t size, size_t nmemb, void* userdata) {
    std::string* response = static_cast<std::string*>(userdata);
    response->append(static_cast<char*>(ptr), size * nmemb);
    return size * nmemb;
}

int main(int argc, char* argv[]) {
    
    // if the anime name isn't entered
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <anime>" << std::endl;
        return 1;
    }

    // seperate
    std::ostringstream oss;
    for (int i = 1; i < argc; ++i) {
        if (i > 1) oss << " ";
        oss << argv[i];
    }

    
    std::string searchTerm = oss.str();
    std::string query = R"({ "query": "query ($search: String) { Page(page: 1, perPage: 10) { media(search: $search, type: ANIME) { id title { romaji english  native } format status episodes duration startDate { year month day } endDate { year month day } season seasonYear averageScore popularity genres} } }",
    "variables": { 
        "search": ")" + searchTerm + R"(" 
    }
})";

    const std::string url = "https://graphql.anilist.co";
    std::string response;

    // pull json data with curl
    CURL* curl = curl_easy_init();
    if (!curl) {
        std::cerr << "Curl başlatılamadı!" << std::endl;
        return 1;
    }

    struct curl_slist* headers = nullptr;
    headers = curl_slist_append(headers, "Content-Type: application/json");

    // Set up the cURL request
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str()); // Set the request URL
    curl_easy_setopt(curl, CURLOPT_POST, 1L); // Set the HTTP method to POST
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, query.c_str()); // Set the POST data (query)
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers); // Set custom HTTP headers
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback); // Set the callback function for handling the response data
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response); // Set the pointer to the response data object


    CURLcode res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
        std::cerr << "Curl err: " << curl_easy_strerror(res) << std::endl;
        return 1;
    }

    curl_easy_cleanup(curl);
    curl_slist_free_all(headers);

    // JSON parsing
    try {
        // parse data
        json jsonResponse = json::parse(response);
        auto media = jsonResponse["data"]["Page"]["media"][0];

        // assign the informations to variables
        int id = media["id"];
        std::string titleRomaji = media["title"]["romaji"];
        std::string titleEnglish = media["title"]["english"];
        std::string titleNative = media["title"]["native"];
        std::string format = media["format"];
        std::string status = media["status"];
        int episodes = media["episodes"];
        int duration = media["duration"];
        int startYear = media["startDate"]["year"];
        int startMonth = media["startDate"]["month"];
        int startDay = media["startDate"]["day"];
        int endYear = media["endDate"]["year"];
        int endMonth = media["endDate"]["month"];
        int endDay = media["endDate"]["day"];
        std::string season = media["season"];
        int seasonYear = media["seasonYear"];
        int averageScore = media["averageScore"];
        int popularity = media["popularity"];
        std::string genres;

        if (media["genres"].is_array()) {
            for (auto& genre : media["genres"]) {
                std::string genreStr = genre.get<std::string>();

                if(!genres.empty()) {
                // different colors for each genre
                    if (genreStr == "Action")
                        genreStr = "\033[38;2;255;102;102m" + genreStr + RESET;
                    else if (genreStr == "Adventure")
                        genreStr = "\033[38;2;255;255;153m" + genreStr + RESET;
                    else if (genreStr == "Comedy")
                        genreStr = "\033[38;2;153;255;153m" + genreStr + RESET;
                    else if (genreStr == "Drama")
                        genreStr = "\033[38;2;153;204;255m" + genreStr + RESET;
                    else if (genreStr == "Ecchi")
                        genreStr = "\033[38;2;255;153;255m" + genreStr + RESET;
                    else if (genreStr == "Fantasy")
                        genreStr = "\033[38;2;204;153;255m" + genreStr + RESET;
                    else if (genreStr == "Horror")
                        genreStr = "\033[38;2;255;0;0m" + genreStr + RESET;
                    else if (genreStr == "Mahou Shoujo")
                        genreStr = "\033[38;2;255;182;193m" + genreStr + RESET;
                    else if (genreStr == "Mecha")
                        genreStr = "\033[38;2;0;0;255m" + genreStr + RESET;
                    else if (genreStr == "Music")
                        genreStr = "\033[38;2;255;228;140m" + genreStr + RESET;
                    else if (genreStr == "Mystery")
                        genreStr = "\033[38;2;102;51;153m" + genreStr + RESET;
                    else if (genreStr == "Psychological")
                        genreStr = "\033[38;2;255;204;0m" + genreStr + RESET;
                    else if (genreStr == "Romance")
                        genreStr = "\033[38;2;255;105;180m" + genreStr + RESET;
                    else if (genreStr == "Sci-Fi")
                        genreStr = "\033[38;2;0;255;255m" + genreStr + RESET;
                    else if (genreStr == "Slice of Life")
                        genreStr = "\033[38;2;255;204;204m" + genreStr + RESET;
                    else if (genreStr == "Sports")
                        genreStr = "\033[38;2;0;255;0m" + genreStr + RESET;
                    else if (genreStr == "Supernatural")
                        genreStr = "\033[38;2;128;0;128m" + genreStr + RESET;
                    else if (genreStr == "Thriller")
                        genreStr = "\033[38;2;255;69;0m" + genreStr + RESET;

                    // separate genres with commas (,)
                     genres += "\033[0m, ";
                }
                genres += genreStr;
            }
        }

        // print infos
        std::cout << BLUE << " Title (Romaji): " << titleRomaji << RESET << "\n";
        std::cout << BLUE << " Title (English): " << titleEnglish << RESET << "\n";
        std::cout << BLUE << " Title (Native): " << titleNative << RESET << "\n";
        std::cout << YELLOW << " Genres: " << genres << RESET << "\n";
        std::cout << CYAN << " Format: " << format << RESET << "\n";
        
        // if the anime is finished, print in green, if not, print in yellow
        if (status == "FINISHED")
            std::cout << GREEN << " Status: " << status << RESET << "\n";
        else
            std::cout << YELLOW << " Status: " << status << RESET << "\n";
        
        std::cout << WHITE << " Episodes: " << episodes << RESET << "\n";
        std::cout << MAGENTA << " Duration: " << duration << RESET << "\n";
        std::cout << YELLOW << " Start Date: " << startDay << "." << startMonth << "." << startYear << RESET << "\n";
        std::cout << YELLOW << " End Date: " << endDay << "." << endMonth << "." << endYear << RESET << "\n";
        std::cout << GREEN << " Season: " << season << RESET << "\n";
        
        // print in green if the score is greater than 80, in yellow if less than 80 but greater than 50 and in red if less than 50
        if (averageScore > 80)
            std::cout << GREEN << " Average Score: " << averageScore << RESET << "\n";
        else if (averageScore >= 50)
            std::cout << YELLOW << " Average Score: " << averageScore << RESET << "\n";
        else if (averageScore < 50)
            std::cout << RED << " Average Score: " << averageScore << RESET << "\n";
        
        std::cout << BLUE << " Popularity: " << popularity << RESET << "\n";



    } catch (const std::exception& e) {
        std::cerr << "JSON parse error: " << e.what() << std::endl;
    }

    return 0;
}
