package org.wikiparser;

import com.google.gson.JsonArray;
import com.google.gson.JsonObject;

public class UrlExecuter {
    public static String execute (JsonObject json_object, Integer num) {

        JsonArray json_array = json_object.getAsJsonObject("query").getAsJsonArray("search");

        if (num >= json_array.size()) {
            throw new IllegalArgumentException("Index " + num + " out of bounds");
        }

        Integer page_id = json_array.get(num).getAsJsonObject().get("pageid").getAsInt();
        return "https://ru.wikipedia.org/w/index.php?curid=" + page_id;
    }
}
