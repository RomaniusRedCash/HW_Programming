TARGET = exe
# Рекурсивно ищем все .cpp файлы, начиная с текущей директории
SOURCES = $(shell find . -name "*.cpp" -not -path "./back/*" )
# Преобразуем пути: например, кладём объектные файлы в папку build, сохраняя структуру
OBJECTS = $(patsubst ./%.cpp, build/%.o, $(SOURCES))

all: release
debug: CXXFLAGS += -g -O0
debug: $(TARGET)
release: CXXFLAGS += -O3 -DNDEBUG
release: $(TARGET)

# Создаём целевой исполняемый файл из всех объектных
$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(BOOST_LIBS)

# Правило компиляции: создаём объектный файл в build/ с сохранением подпапок
build/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf build $(TARGET)

.PHONY: clean
