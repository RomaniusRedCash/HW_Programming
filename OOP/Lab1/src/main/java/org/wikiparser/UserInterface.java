package org.wikiparser;

import com.google.gson.JsonArray;
import com.google.gson.JsonObject;
import java.util.Scanner;

public class UserInterface {
    private static final Scanner scanner = new Scanner(System.in);

    public static String getReq() {
        String input;
        do {
            System.out.println("Введите запрос: ");
            input = scanner.nextLine().trim();
            if (input.isEmpty())
                System.out.println("Ввод некорректен!");
        } while (input.isEmpty());
        return input;
    }

    public static void outJson(JsonObject jsonObject) {

        System.out.println("Найдено:");

        JsonArray json_array = jsonObject.getAsJsonObject("query").getAsJsonArray("search");

        for (Integer i = 0; i < json_array.size(); i++) {
            System.out.println(i + ": " + json_array.get(i).getAsJsonObject().get("title").getAsString());
        }
    }

    public static Integer getId() {
        System.out.println("Введите искомый id: ");
        if (!scanner.hasNextInt()){
            scanner.next();
            return -1;
        }
        return scanner.nextInt();
    }
}
