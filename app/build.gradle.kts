//plugins {
//    alias(libs.plugins.android.application)
//}

android {
    namespace = "com.example.test_android_kanji"
    compileSdk = 35

    signingConfigs {
        getByName("debug") {
            storeFile = file("keystores/tf-android-keystore.jks")
            storePassword = "GrapeApplePlum4$@TF"
            keyPassword = "AppleC@tGrape$4@TF"
            keyAlias = "key0"
        }
    }

    buildFeatures {
        buildConfig = true
        viewBinding = true
    }

    defaultConfig {
        applicationId = "com.example.test_android_kanji"
        minSdk = 28
        targetSdk = 35
        versionCode = 1
        versionName = "1.0"
        testInstrumentationRunner = "androidx.test.runner.AndroidJUnitRunner"

        buildConfigField("int", "CURRENT_MAIN_OBB_VERSION_IN_STORE", "1")
        buildConfigField("long", "CURRENT_MAIN_OBB_VERSION_FILESIZE", "492596890L")
        buildConfigField("boolean", "GAME_DEBUG_ENABLE", "false")
        buildConfigField("int", "GAME_DEBUG_RES", "0")
        buildConfigField("boolean", "GAME_SPLASH_SKIP", "false")
        buildConfigField("boolean", "GAME_SURVEY_BUILD", "false")
    }

    buildTypes {
        getByName("release") {
            isDebuggable = true
            isMinifyEnabled = false
            proguardFiles(
                getDefaultProguardFile("proguard-android-optimize.txt"),
                "proguard-rules.pro"
            )
            buildConfigField("String", "BUILD_VARIANT", "\"google\"")
        }

        debug {
            isDebuggable = true
            isMinifyEnabled = false
            isJniDebuggable = true
            buildConfigField("String", "BUILD_VARIANT", "\"google\"")
        }

        create("amazon_release") {
            isDebuggable = false
            isMinifyEnabled = false
            buildConfigField("String", "BUILD_VARIANT", "\"amazon\"")
        }

        create("amazon_debug") {
            isDebuggable = true
            isMinifyEnabled = false
            isJniDebuggable = true
            buildConfigField("String", "BUILD_VARIANT", "\"amazon\"")
            signingConfig = signingConfigs.getByName("debug")
        }
    }

    compileOptions {
        sourceCompatibility = JavaVersion.VERSION_1_8
        targetCompatibility = JavaVersion.VERSION_1_8
    }

    externalNativeBuild {
        cmake {
            path = file("src/main/cpp/CMakeLists.txt")
            version = "3.22.1"
        }
    }
}

dependencies {
//    implementation(libs.kotlin.gradle.plugin)
//    implementation(libs.appcompat.v7)
//    implementation(libs.kotlin.stdlib)
    implementation(libs.credentials)
    implementation(libs.credentials.play.services.auth)
    implementation(libs.play.services.games.v2)
    implementation(libs.play.services.location)
    implementation(libs.play.services.auth)
    implementation(libs.github.fetch2okhttp)
    implementation(libs.fetch2)
    implementation(libs.appcompat)
    implementation(libs.material)
    implementation(libs.constraintlayout)
    implementation(libs.play.services.tasks)
    implementation(libs.firebase.storage)
    implementation(libs.play.services.measurement.api)
    implementation(libs.firebase.auth)
    implementation(libs.firebase.database)
    testImplementation(libs.junit)
}
