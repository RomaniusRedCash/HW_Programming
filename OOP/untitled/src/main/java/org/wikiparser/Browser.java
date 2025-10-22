package org.wikiparser;

public class Browser {
    public static void open(String url) {

        try {
            if (java.awt.Desktop.isDesktopSupported()) {
                java.awt.Desktop desktop = java.awt.Desktop.getDesktop();
                if (desktop.isSupported(java.awt.Desktop.Action.BROWSE)) {
                    desktop.browse(new java.net.URI(url));
                    return;
                }
            }

            System.out.println("Desktop.browse() not supported, using fallback...");

            try {
                Runtime.getRuntime().exec(new String[]{"xdg-open", url});
                return;
            } catch (Exception e) {
            }

            String[] browsers = {"firefox", "chromium", "google-chrome", "opera"};
            for (String browser : browsers) {
                try {
                    Runtime.getRuntime().exec(new String[]{browser, url});
                    return;
                } catch (Exception e) {
                    continue;
                }
            }

            System.out.println("Please open this URL manually: " + url);

        } catch (Exception e) {
            System.err.println("Failed to open URL: " + url);
            e.printStackTrace();
        }
    }
}
