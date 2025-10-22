package org.wikiparser;

import com.google.gson.JsonArray;
import com.google.gson.JsonObject;

public class UrlExecuter {
    private String url;
    public void execute (JsonObject jsonObject, Integer num) {

        JsonArray jsonArray = jsonObject.getAsJsonObject("query").getAsJsonArray("search");

        if (num >= jsonArray.size()) {
            throw new IllegalArgumentException("Index " + num + " out of bounds");
        }

        Integer page_id = jsonArray.get(num).getAsJsonObject().get("pageid").getAsInt();
        url = "https://ru.wikipedia.org/w/index.php?curid=" + page_id;
    }
    public String getUrl() {return url;}
}
