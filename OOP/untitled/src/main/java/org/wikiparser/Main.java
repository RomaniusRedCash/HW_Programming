package org.wikiparser;

import java.io.IOException;
import java.net.URISyntaxException;

public class Main {
    private static final Requester requester = new Requester();
    private static final UrlExecuter executer = new UrlExecuter();

    public static void main(String[] args) throws URISyntaxException, IOException, InterruptedException {
        System.setOut(new java.io.PrintStream(System.out, true, "UTF-8"));
        System.setProperty("console.encoding", "UTF-8");

        requester.request("https://ru.wikipedia.org/w/api.php?action=query&list=search&utf8=&format=json&srsearch=" , UserInterface.getReq());

        UserInterface.outJson(requester.getJsonObject());

        while(!executer.execute(requester.getJsonObject(), UserInterface.getId())){
            System.out.println("Данные не верны!");
        };

        Browser.open(executer.getUrl());
    }
}
