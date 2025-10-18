package org.wikiparser;

import com.google.gson.Gson;
import com.google.gson.JsonObject;
import com.google.gson.JsonParser;

public class JsonCreater {
    public static JsonObject makegson(String string){
        return JsonParser.parseString(string).getAsJsonObject();
    }
}
