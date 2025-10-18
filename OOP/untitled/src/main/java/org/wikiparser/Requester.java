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
    public static JsonObject request(String url_string, String req_string) throws URISyntaxException, IOException, InterruptedException {
        req_string = URLEncoder.encode(req_string, "UTF-8");
        final HttpURLConnection con = (HttpURLConnection) new URL(url_string + req_string).openConnection();
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
            return JsonParser.parseString(content.toString()).getAsJsonObject();

    }
}
