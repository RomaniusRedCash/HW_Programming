plugins {
    id("java")
    id("application")  // ← добавляем этот плагин
}

application {
    mainClass.set("org.wikiparser.Main")  // укажите ваш главный класс
    applicationDefaultJvmArgs = listOf("-Dfile.encoding=UTF-8")
}

group = "org.example"
version = "1.0-SNAPSHOT"

repositories {
    mavenCentral()
}

dependencies {
    testImplementation(platform("org.junit:junit-bom:5.10.0"))
    testImplementation("org.junit.jupiter:junit-jupiter")

    implementation("com.google.code.gson:gson:2.10.1")

}

tasks.test {
    useJUnitPlatform()
}
tasks.withType<JavaExec> {
    standardInput = System.`in`
    systemProperty("file.encoding", "UTF-8")
}

tasks.withType<JavaCompile> {
    options.encoding = "UTF-8"
}

tasks.withType<Test> {
    systemProperty("file.encoding", "UTF-8")
}