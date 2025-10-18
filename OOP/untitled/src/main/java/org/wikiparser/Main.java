package org.wikiparser;

import com.google.gson.JsonObject;

import java.io.IOException;
import java.net.URI;
import java.net.URISyntaxException;

public class Main {
    public static void main(String[] args) throws URISyntaxException, IOException, InterruptedException {
        System.setOut(new java.io.PrintStream(System.out, true, "UTF-8"));
        System.setProperty("console.encoding", "UTF-8");


        String req_serch = UserInterface.getReq();

        JsonObject json_object = Requester.request("https://ru.wikipedia.org/w/api.php?action=query&list=search&utf8=&format=json&srsearch=" , req_serch);
        UserInterface.outJson(json_object);

        Integer id = UserInterface.getId();

        String url = UrlExecuter.execute(json_object, id);
        java.awt.Desktop.getDesktop().browse(new URI(url));
    }
}