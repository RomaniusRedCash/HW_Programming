package org.wikiparser;

import com.google.gson.JsonArray;
import com.google.gson.JsonObject;

public class UrlExecuter {
    private String url;
    public Boolean execute (JsonObject jsonObject, Integer num) {
        JsonArray jsonArray = jsonObject.getAsJsonObject("query").getAsJsonArray("search");

        if (num < 0 || num >= jsonArray.size()) {
            return Boolean.FALSE;
        }

        Integer page_id = jsonArray.get(num).getAsJsonObject().get("pageid").getAsInt();
        url = "https://ru.wikipedia.org/w/index.php?curid=" + page_id;
        return Boolean.TRUE;
    }
    public String getUrl() {return url;}
}
