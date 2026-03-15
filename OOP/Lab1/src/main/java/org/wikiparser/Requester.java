package org.wikiparser;

import com.google.gson.JsonObject;
import com.google.gson.JsonParseException;
import com.google.gson.JsonParser;
import java.io.IOException;
import java.net.URI;
import java.net.URISyntaxException;
import java.net.URLEncoder;
import java.net.http.HttpClient;
import java.net.http.HttpRequest;
import java.net.http.HttpResponse;
import java.time.Duration;



public class Requester {
    private JsonObject jsonObject;



    public void request(String urlString, String reqString) throws URISyntaxException, IOException, InterruptedException {
        reqString = URLEncoder.encode(reqString, "UTF-8");
        HttpClient client = HttpClient.newHttpClient();
        HttpRequest request = HttpRequest.newBuilder().uri(URI.create(urlString + reqString)).header("Content-Type", "application/json").header("User-Agent", "WikiParser/1.0").GET().timeout(Duration.ofSeconds(20)).build();
        HttpResponse<String> response;
        try {
            response = client.send(request, HttpResponse.BodyHandlers.ofString());
            Integer statusCode = response.statusCode();
            if (statusCode != 200) {
                throw new IOException("HTTP error: " + statusCode + " - " + response.body());
            }
        } catch (IOException e) {
            throw e;
        }

        try{
            jsonObject = JsonParser.parseString(response.body()).getAsJsonObject();
        }
        catch (JsonParseException e){
            throw e;
        }


    }

    public JsonObject getJsonObject() {return jsonObject;}
}
