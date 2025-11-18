package org.example;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

class FloorData {
    public Map<Integer, Integer> data = new HashMap<>();
    public void add(Integer floor) {
        data.merge(floor, 1, Integer::sum);
    }
}

class HomeData {
    public Map<Integer, FloorData> data = new HashMap<>();
    public void add(Integer home, Integer floor) {
        data.computeIfAbsent(home, k -> new FloorData()).add(floor);
    }
}

class StreetData {
    public Map<String, HomeData> data = new HashMap<>();
    public void add(String street, Integer home, Integer floor) {
        data.computeIfAbsent(street, k -> new HomeData()).add(home, floor);
    }
}

class SubCityData {
    public StreetData streets = new StreetData();
    public Map<Integer, Integer> floors = new HashMap();
    public void add(String street, Integer home, Integer floor) {
        streets.add(street, home, floor);
        floors.merge(floor, 1, Integer::sum);
    }
}

class CityData {
    public Map<String, SubCityData> data = new HashMap<>();
    public void add(String city, String street, Integer home, Integer floor) {
        data.computeIfAbsent(city, k -> new SubCityData()).add(street, home, floor);
    }
}