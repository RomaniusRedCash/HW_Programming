package org.wikiparser;

import com.google.gson.JsonObject;
import com.google.gson.JsonParser;


import java.io.BufferedReader;
import java.io.IOException;

import java.io.InputStreamReader;
import java.net.HttpURLConnection;
import java.net.URISyntaxException;
import java.net.URL;
import java.net.URLEncoder;



public class Requester {
    private JsonObject jsonObject;



    public void request(String urlString, String reqString) throws URISyntaxException, IOException, InterruptedException {
        reqString = URLEncoder.encode(reqString, "UTF-8");
        final HttpURLConnection con = (HttpURLConnection) new URL(urlString + reqString).openConnection();
        con.setRequestMethod("GET");
        con.setRequestProperty("Accept", "application/json");
        con.setRequestProperty("User-Agent", "WikiParserBot/1.0 (https://example.com; contact@example.com)");
        con.setConnectTimeout(5000);
        con.setReadTimeout(5000);


        final BufferedReader in = new BufferedReader(new InputStreamReader(con.getInputStream()));
        String inputLine;
        final StringBuilder content = new StringBuilder();
        while ((inputLine = in.readLine()) != null) {
            content.append(inputLine);
        }
        in.close();
        jsonObject = JsonParser.parseString(content.toString()).getAsJsonObject();
    }

    public JsonObject getJsonObject() {return jsonObject;}
}
