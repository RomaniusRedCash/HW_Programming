package org.wikiparser;

import com.google.gson.JsonArray;
import com.google.gson.JsonObject;

import java.util.Scanner;

public class UserInterface {
    private static final Scanner scanner = new Scanner(System.in);

    public static String getReq() {
        System.out.println("Введите запрос: ");
        return scanner.nextLine();
    }

    public static void outJson(JsonObject json_object) {

        System.out.println("Найдено:");

        JsonArray json_array = json_object.getAsJsonObject("query").getAsJsonArray("search");

        for (Integer i = 0; i < json_array.size(); i++) {
            System.out.println(i + ": " + json_array.get(i).getAsJsonObject().get("title").getAsString());
        }
    }

    public static Integer getId() {
        System.out.println("Введите искомый id: ");
        return scanner.nextInt();
    }
}
