#include <future>
#include <time.h>
#include <sstream>
#include <string>
#include <fstream>

#include <curl/curl.h>
#include <jsoncpp/json/json.h>
#include <tinyxml2.h>

// Please configure me
const std::string feedUrl = "/feeds/";
const std::string domain = "example.com";
const std::string feedPath = "../feeds/";
const std::string regionsFilePath = "../regions.json";

// Do not touch unless you know what you are doing
struct Region {
    std::string abbreviation;
    std::string name;
};

struct Response {
    long status;
    Region region;
    std::string body;
};

std::string replace(std::string& str, const std::string& from, const std::string& to) {
    size_t start_pos = str.find(from);
    if(start_pos == std::string::npos) return str;
    return str.replace(start_pos, from.length(), to);
}

static size_t write_data(void *contents, size_t size, size_t nmemb, void *userp) {
   ((std::string*)userp)->append((char*)contents, size * nmemb);
   return size * nmemb;
}

Response fetchContentForRegion(Region region) {
    Response response;
    std::string readBuffer;
    char params[50];

    response.status = 0;


    CURL *curl = curl_easy_init();

    sprintf(params, "location=%s", curl_easy_escape(curl, region.abbreviation.c_str(), strlen(region.abbreviation.c_str())));

    std::stringstream url;
    url << "https://www.biwapp.de/widget/dataBiwappProxy" << "?" << params;

    if(curl) {
        struct curl_slist *headers = NULL;
        headers = curl_slist_append(headers, "User-Agent: Mozilla/5.0 (Macintosh; Intel Mac OS X 10.15; rv:73.0) Gecko/20100101 Firefox/73.0");
        headers = curl_slist_append(headers, "Accept: */*");

        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_URL, url.str().c_str());
        curl_easy_setopt(curl, CURLOPT_TIMEOUT, 30);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &write_data);
        CURLcode res = curl_easy_perform(curl);
        curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response.status);
        curl_easy_cleanup(curl);

        response.body = readBuffer;
    }

    return response;
}

const std::string getDateTime(time_t timeStamp) {
    time_t now = timeStamp;
    struct tm tstruct;
    char buf[80];
    tstruct = *localtime(&now);
    strftime(buf, sizeof(buf), "%Y-%m-%dT%XZ", &tstruct);

    return buf;
}

void generateFeed(Region region) {
    Response response = fetchContentForRegion(region);

    Json::Value jsonRoot;
    std::stringstream stream;
    stream << response.body;
    stream >> jsonRoot;

    Json::Value jsonNews = jsonRoot["news"];

    std::stringstream filename;
    filename << feedPath << region.abbreviation << ".xml";

    tinyxml2::XMLDocument xmlDocument;
    tinyxml2::XMLDeclaration *declaration = xmlDocument.NewDeclaration("xml version=\"1.0\" encoding=\"utf-8\"");
    xmlDocument.InsertFirstChild(declaration);
    tinyxml2::XMLElement *xmlRoot = xmlDocument.NewElement("feed");
    xmlRoot->SetAttribute("xmlns", "http://www.w3.org/2005/Atom");
    xmlRoot->SetAttribute("xml:lang", "de");

    tinyxml2::XMLElement *xmlTitle = xmlDocument.NewElement("title");
    xmlTitle->SetText(region.name.c_str());

    std::stringstream linkFeedStream;
    linkFeedStream << "http://" << domain << feedUrl << region.abbreviation << ".xml";

    tinyxml2::XMLElement *xmlFeedLink = xmlDocument.NewElement("link");
    xmlFeedLink->SetAttribute("rel", "self");
    xmlFeedLink->SetAttribute("type", "application/atom+xml");
    xmlFeedLink->SetAttribute("href", linkFeedStream.str().c_str());

    tinyxml2::XMLElement *xmlId = xmlDocument.NewElement("id");
    xmlId->SetText(linkFeedStream.str().c_str());

    tinyxml2::XMLElement *xmlGenerator = xmlDocument.NewElement("generator");
    xmlGenerator->SetAttribute("version", "0.1");
    xmlGenerator->SetText("Blast");

    tinyxml2::XMLElement *xmlTimeStamp = xmlDocument.NewElement("updated");
    xmlTimeStamp->SetText(getDateTime(time(0)).c_str());

    xmlRoot->InsertEndChild(xmlTitle);
    xmlRoot->InsertEndChild(xmlFeedLink);
    xmlRoot->InsertEndChild(xmlId);
    xmlRoot->InsertEndChild(xmlTimeStamp);
    xmlRoot->InsertEndChild(xmlGenerator);
    xmlDocument.InsertEndChild(xmlRoot);

    for(int i = 0; i < jsonNews.size(); i++) {
        tinyxml2::XMLElement *xmlEntry = xmlDocument.NewElement("entry");

        tinyxml2::XMLElement *xmlEntryId = xmlDocument.NewElement("id");
        std::stringstream idStream;
        int id = jsonNews[i].get("id", "").asInt();
        idStream << linkFeedStream.str() << "," << std::to_string(id);
        xmlEntryId->SetText(idStream.str().c_str());

        tinyxml2::XMLElement *xmlEntryTitle = xmlDocument.NewElement("title");
        xmlEntryTitle->SetText(jsonNews[i].get("title", "").asCString());

        tinyxml2::XMLElement *xmlEntryAuthorName = xmlDocument.NewElement("name");
        xmlEntryAuthorName->SetText(jsonNews[i].get("sender", "").asCString());

        tinyxml2::XMLElement *xmlEntryAuthor = xmlDocument.NewElement("author");
        xmlEntryAuthor->InsertEndChild(xmlEntryAuthorName);

        tinyxml2::XMLElement *xmlEntryCategory = xmlDocument.NewElement("category");
        xmlEntryCategory->SetAttribute("term", jsonNews[i].get("categoryIcon", "").asCString());
        xmlEntryCategory->SetAttribute("label", jsonNews[i].get("categoryIcon", "").asCString());

        tinyxml2::XMLElement *xmlEntrySummary = xmlDocument.NewElement("summary");
        xmlEntrySummary->SetText(jsonNews[i].get("shortdescription", "").asCString());

        tinyxml2::XMLElement *xmlEntryContent = xmlDocument.NewElement("content");
        xmlEntryContent->SetAttribute("type", "text/html");
        xmlEntryContent->SetText(jsonNews[i].get("details", "").asCString());

        tinyxml2::XMLElement *xmlEntryTimeStamp = xmlDocument.NewElement("published");
        struct tm tm;
        strptime(jsonNews[i].get("valid_from", "").asCString(), "%d.%m.%Y %H:%M:%S", &tm);
        time_t t = mktime(&tm);
        xmlEntryTimeStamp->SetText(getDateTime(t).c_str());

        xmlEntry->InsertEndChild(xmlEntryId);
        xmlEntry->InsertEndChild(xmlEntryTitle);
        xmlEntry->InsertEndChild(xmlEntryAuthor);
        xmlEntry->InsertEndChild(xmlEntryCategory);
        xmlEntry->InsertEndChild(xmlEntrySummary);
        xmlEntry->InsertEndChild(xmlEntryContent);
        xmlEntry->InsertEndChild(xmlEntryTimeStamp);
        xmlRoot->InsertEndChild(xmlEntry);
    }

    xmlDocument.SaveFile(filename.str().c_str());
}

void generateAllFeeds() {
    Json::Value jsonRoot;

    std::ifstream regionsFile(regionsFilePath, std::ifstream::binary);
    regionsFile >> jsonRoot;

    std::future<void> f[jsonRoot.size()];

    for(int j = 0; j < jsonRoot.size(); j++) {
        f[j] = std::async(std::launch::async, [jsonRoot, j]{
            printf("Generating feed for %s\n", jsonRoot[j].get("abbreviation", "").asCString());

            Region region;
            region.name = jsonRoot[j].get("name", "").asString();
            region.abbreviation = jsonRoot[j].get("abbreviation", "").asString();
            generateFeed(region);
        });
    }
}

int main() {
    generateAllFeeds();
    return 0;
}
