package org.wikiparser;

import java.io.IOException;
import java.net.URISyntaxException;

public class Browser {
    public static void open(String url) throws URISyntaxException, IOException {
        if (java.awt.Desktop.isDesktopSupported() && java.awt.Desktop.getDesktop().isSupported(java.awt.Desktop.Action.BROWSE)) {
                java.awt.Desktop.getDesktop().browse(new java.net.URI(url));
        } else {
            new ProcessBuilder("xdg-open", url).start();
            // Runtime.getRuntime().exec("xdg-open " + url);
        }
    }
}