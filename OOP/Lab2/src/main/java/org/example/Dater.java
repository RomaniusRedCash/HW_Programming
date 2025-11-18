import java.util.HashMap;
import java.util.Map;

class HomeStr {
    public int floors;
    public int repeats = 0;

    public HomeStr(int floors) {
        this.floors = floors;
    }

    public HomeStr(int floors, int repeats) {
        this.floors = floors;
        this.repeats = repeats;
    }
}

class City {
    public Map<String, Map<Integer, HomeStr>> subData = new HashMap<>();
    public Map<Integer, Integer> floorsMap = new HashMap<>();

    public Map<Integer, HomeStr> get(String street) {
        return subData.getOrDefault(street, new HashMap<>());
    }

    public Map<Integer, HomeStr> getOrCreate(String street) {
        return subData.computeIfAbsent(street, k -> new HashMap<>());
    }
}

public class Dater {
    public Map<String, City> data = new HashMap<>();

    public City get(String city) {
        return data.getOrDefault(city, new City());
    }

    public City getOrCreate(String city) {
        return data.computeIfAbsent(city, k -> new City());
    }

    public void add(String city, String street, int house, int floors) {
        City cityObj = getOrCreate(city);
        Map<Integer, HomeStr> streetData = cityObj.getOrCreate(street);

        HomeStr houseStr = streetData.get(house);
        if (houseStr == null) {
            houseStr = new HomeStr(floors);
            streetData.put(house, houseStr);

            // Обновляем floorsMap
            cityObj.floorsMap.put(floors, cityObj.floorsMap.getOrDefault(floors, 0) + 1);
        }
        /* else if (houseStr.floors != floors) {
            throw new RuntimeException("Два дома с одинаковым адресом");
        } */

        houseStr.repeats++;
    }
}