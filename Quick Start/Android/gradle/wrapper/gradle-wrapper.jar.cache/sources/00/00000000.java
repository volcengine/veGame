package org.gradle.wrapper;

import java.io.File;
import java.io.InputStream;
import java.net.URI;
import java.net.URISyntaxException;
import java.util.HashMap;
import java.util.Map;
import java.util.Properties;
import org.gradle.cli.CommandLineParser;
import org.gradle.cli.ParsedCommandLine;
import org.gradle.cli.SystemPropertiesCommandLineConverter;

/* loaded from: gradle-wrapper.jar:org/gradle/wrapper/GradleWrapperMain.class */
public class GradleWrapperMain {
    public static final String GRADLE_USER_HOME_OPTION = "g";
    public static final String GRADLE_USER_HOME_DETAILED_OPTION = "gradle-user-home";
    public static final String GRADLE_QUIET_OPTION = "q";
    public static final String GRADLE_QUIET_DETAILED_OPTION = "quiet";

    public static void main(String[] args) throws Exception {
        File wrapperJar = wrapperJar();
        File propertiesFile = wrapperProperties(wrapperJar);
        File rootDir = rootDir(wrapperJar);
        CommandLineParser parser = new CommandLineParser();
        parser.allowUnknownOptions();
        parser.option(GRADLE_USER_HOME_OPTION, GRADLE_USER_HOME_DETAILED_OPTION).hasArgument();
        parser.option(GRADLE_QUIET_OPTION, GRADLE_QUIET_DETAILED_OPTION);
        SystemPropertiesCommandLineConverter converter = new SystemPropertiesCommandLineConverter();
        converter.configure(parser);
        ParsedCommandLine options = parser.parse(args);
        Properties systemProperties = System.getProperties();
        systemProperties.putAll(converter.convert(options, (Map<String, String>) new HashMap()));
        File gradleUserHome = gradleUserHome(options);
        addSystemProperties(gradleUserHome, rootDir);
        Logger logger = logger(options);
        WrapperExecutor wrapperExecutor = WrapperExecutor.forWrapperPropertiesFile(propertiesFile);
        wrapperExecutor.execute(args, new Install(logger, new Download(logger, "gradlew", wrapperVersion()), new PathAssembler(gradleUserHome)), new BootstrapMainStarter());
    }

    private static void addSystemProperties(File gradleHome, File rootDir) {
        System.getProperties().putAll(SystemPropertiesHandler.getSystemProperties(new File(gradleHome, "gradle.properties")));
        System.getProperties().putAll(SystemPropertiesHandler.getSystemProperties(new File(rootDir, "gradle.properties")));
    }

    private static File rootDir(File wrapperJar) {
        return wrapperJar.getParentFile().getParentFile().getParentFile();
    }

    private static File wrapperProperties(File wrapperJar) {
        return new File(wrapperJar.getParent(), wrapperJar.getName().replaceFirst("\\.jar$", ".properties"));
    }

    private static File wrapperJar() {
        try {
            URI location = GradleWrapperMain.class.getProtectionDomain().getCodeSource().getLocation().toURI();
            if (!location.getScheme().equals("file")) {
                throw new RuntimeException(String.format("Cannot determine classpath for wrapper Jar from codebase '%s'.", location));
            }
            return new File(location.getPath());
        } catch (URISyntaxException e) {
            throw new RuntimeException(e);
        }
    }

    static String wrapperVersion() {
        try {
            InputStream resourceAsStream = GradleWrapperMain.class.getResourceAsStream("/build-receipt.properties");
            if (resourceAsStream == null) {
                throw new RuntimeException("No build receipt resource found.");
            }
            Properties buildReceipt = new Properties();
            buildReceipt.load(resourceAsStream);
            String versionNumber = buildReceipt.getProperty("versionNumber");
            if (versionNumber == null) {
                throw new RuntimeException("No version number specified in build receipt resource.");
            }
            resourceAsStream.close();
            return versionNumber;
        } catch (Exception e) {
            throw new RuntimeException("Could not determine wrapper version.", e);
        }
    }

    private static File gradleUserHome(ParsedCommandLine options) {
        if (options.hasOption(GRADLE_USER_HOME_OPTION)) {
            return new File(options.option(GRADLE_USER_HOME_OPTION).getValue());
        }
        return GradleUserHomeLookup.gradleUserHome();
    }

    private static Logger logger(ParsedCommandLine options) {
        return new Logger(options.hasOption(GRADLE_QUIET_OPTION));
    }
}