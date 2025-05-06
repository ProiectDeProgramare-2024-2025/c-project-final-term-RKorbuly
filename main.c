#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_TITLE 100
#define MAX_CONTENT 500
#define MAX_ARTICLES 50
#define FILE_NAME "articles.txt"
#define COLOR_RESET   "\033[0m"
#define COLOR_RED     "\033[31m"
#define COLOR_GREEN   "\033[32m"
#define COLOR_YELLOW  "\033[33m"
#define COLOR_BLUE    "\033[34m"
#define COLOR_MAGENTA "\033[35m"
#define COLOR_CYAN    "\033[36m"

typedef struct {
    char title[MAX_TITLE];
    char content[MAX_CONTENT];
    int is_favorite;
} Article;

Article articles[MAX_ARTICLES];
int article_count = 0;

void saveArticlesToFile() {
    FILE *fp = fopen(FILE_NAME, "w");
    if (fp == NULL) {
        printf("Error opening file for writing.\n");
        return;
    }
    fprintf(fp, "%d\n", article_count);
    for (int i = 0; i < article_count; i++) {
        fprintf(fp, "%s\n", articles[i].title);
        fprintf(fp, "%s\n", articles[i].content);
        fprintf(fp, "%d\n", articles[i].is_favorite);
    }
    fclose(fp);
}

void loadArticlesFromFile() {
    FILE *fp = fopen(FILE_NAME, "r");
    if (fp == NULL) {
        article_count = 0;
        return;
    }

    char line[512];
    if (fgets(line, sizeof(line), fp) != NULL)
        article_count = atoi(line);
    else
        article_count = 0;

    for (int i = 0; i < article_count; i++) {
        if (fgets(articles[i].title, MAX_TITLE, fp))
            articles[i].title[strcspn(articles[i].title, "\n")] = '\0';
        if (fgets(articles[i].content, MAX_CONTENT, fp))
            articles[i].content[strcspn(articles[i].content, "\n")] = '\0';
        if (fgets(line, sizeof(line), fp))
            articles[i].is_favorite = atoi(line);
    }
    fclose(fp);
}

void addArticle() {
    printf("\e[1;1H\e[2J");
    printf("=== %sAdd Article%s ===\n", COLOR_CYAN, COLOR_RESET);
    if (article_count >= MAX_ARTICLES) {
        printf("Article limit reached!\n");
        printf("Press" COLOR_YELLOW " Enter" COLOR_RESET " to continue");
        getchar();
        return;
    }

    printf("Enter " COLOR_GREEN "article title: " COLOR_RESET);
    fgets(articles[article_count].title, MAX_TITLE, stdin);
    articles[article_count].title[strcspn(articles[article_count].title, "\n")] = '\0';

    printf("Enter " COLOR_GREEN "article content: "COLOR_RESET);
    fgets(articles[article_count].content, MAX_CONTENT, stdin);
    articles[article_count].content[strcspn(articles[article_count].content, "\n")] = '\0';

    articles[article_count].is_favorite = 0;
    article_count++;

    printf("Article %sadded%s successfully!\n", COLOR_GREEN, COLOR_RESET);
    printf("Press Enter to continue");
    getchar();
}

void viewArticles() {
    printf("\e[1;1H\e[2J");
    printf("=== %sViewing Articles%s ===\n", COLOR_CYAN, COLOR_RESET);
    if (article_count == 0) {
        printf("No articles available.\n");
    } else {
        for (int i = 0; i < article_count; i++) {
            printf("\nArticle %d:\n", i + 1);
            printf("%sTitle%s: %s\n", COLOR_BLUE, COLOR_RESET, articles[i].title);
            printf("%sContent%s: %s\n", COLOR_BLUE, COLOR_RESET, articles[i].content);
            printf("%s\n", articles[i].is_favorite ? "Favorite: " COLOR_MAGENTA "Yes" COLOR_RESET : "Favorite:"  COLOR_MAGENTA " No" COLOR_RESET);
        }
    }
    printf("Press" COLOR_YELLOW " Enter" COLOR_RESET " to continue");
    getchar();
}

void searchArticle() {
    printf("\e[1;1H\e[2J");
    printf("=== %sSearching Articles%s ===\n", COLOR_CYAN, COLOR_RESET);
    char keyword[MAX_TITLE];
    printf("Enter keyword to search in titles: ");
    scanf("%s", keyword);
    getchar();

    int found = 0;
    for (int i = 0; i < article_count; i++) {
        if (strstr(articles[i].title, keyword)) {
            printf("\nArticle %d:\n", i + 1);
            printf("%sTitle%s: %s\n", COLOR_BLUE, COLOR_RESET, articles[i].title);
            printf("%sContent%s: %s\n", COLOR_BLUE, COLOR_RESET, articles[i].content);
            printf("%s\n", articles[i].is_favorite ? "Favorite: " COLOR_MAGENTA "Yes" COLOR_RESET : "Favorite:"  COLOR_MAGENTA " No" COLOR_RESET);
            found = 1;
        }
    }
    if (!found)
        printf("No articles found with the keyword '%s%s%s'.\n", COLOR_YELLOW, keyword, COLOR_RESET);
    printf("Press" COLOR_YELLOW " Enter" COLOR_RESET " to continue");
    getchar();
}

void favoriteArticle() {
    printf("\e[1;1H\e[2J");
    printf("=== %sFavorite Article%s ===\n", COLOR_CYAN, COLOR_RESET);
    int index;
    printf("Enter article number to mark as favorite: ");
    scanf("%d", &index);
    getchar();

    if (index < 1 || index > article_count) {
        printf("Invalid article number.\n");
        printf("Press" COLOR_YELLOW " Enter" COLOR_RESET " to continue");
        getchar();
        return;
    }

    articles[index - 1].is_favorite = 1;
    printf("Article marked as %sfavorite%s!\n", COLOR_MAGENTA, COLOR_RESET);
    printf("Press" COLOR_YELLOW " Enter" COLOR_RESET " to continue");
    getchar();
}

void viewFavorites() {
    printf("\e[1;1H\e[2J");
    printf("=== %sViewing Favorite Articles%s ===\n", COLOR_CYAN, COLOR_RESET);
    int found = 0;
    for (int i = 0; i < article_count; i++) {
        if (articles[i].is_favorite) {
            printf("\nArticle %d:\n", i + 1);
            printf("%sTitle%s: %s\n", COLOR_BLUE, COLOR_RESET, articles[i].title);
            printf("%sContent%s: %s\n", COLOR_BLUE, COLOR_RESET, articles[i].content);
            found = 1;
        }
    }
    if (!found)
        printf("No %sfavorite%s articles found.\n", COLOR_MAGENTA, COLOR_RESET);
    printf("Press" COLOR_YELLOW " Enter" COLOR_RESET " to continue");
    getchar();
}

void viewFile() {
    printf("\e[1;1H\e[2J");
    FILE *fp = fopen(FILE_NAME, "r");
    if (fp == NULL) {
        printf("No %spersistent%s file found.\n", COLOR_RED, COLOR_RESET);
        printf("Press" COLOR_YELLOW " Enter" COLOR_RESET " to continue");
        getchar();
        return;
    }

    char line[512];
    int fileArticleCount = 0;
    if (fgets(line, sizeof(line), fp))
        fileArticleCount = atoi(line);

    printf("=== %sViewing Articles from file%s ===\n", COLOR_CYAN, COLOR_RESET);
    for (int i = 0; i < fileArticleCount; i++) {
        char title[MAX_TITLE] = "";
        char content[MAX_CONTENT] = "";
        int fav = 0;

        fgets(title, MAX_TITLE, fp);
        fgets(content, MAX_CONTENT, fp);
        fgets(line, sizeof(line), fp);
        fav = atoi(line);

        printf("\nArticle %d:\n", i + 1);
        printf("%sTitle%s: %s\n", COLOR_BLUE, COLOR_RESET, articles[i].title);
        printf("%sContent%s: %s\n", COLOR_BLUE, COLOR_RESET, articles[i].content);
        printf("%s\n", articles[i].is_favorite ? "Favorite: " COLOR_MAGENTA "Yes" COLOR_RESET : "Favorite:"  COLOR_MAGENTA " No" COLOR_RESET);
    }
    fclose(fp);
    printf("Press" COLOR_YELLOW " Enter" COLOR_RESET " to continue");
    getchar();
}

void manualSave() {
    printf("\e[1;1H\e[2J");
    saveArticlesToFile();
    printf("Articles %ssaved%s to file successfully.\n", COLOR_GREEN, COLOR_RESET);
    printf("Press" COLOR_YELLOW " Enter" COLOR_RESET " to continue");
    getchar();
}

int main() {
    int choice;
    loadArticlesFromFile();

    do {
        printf("\e[1;1H\e[2J");
        printf("=== %sMain Menu%s ===\n", COLOR_BLUE, COLOR_RESET);
        printf("\n1. Add Article\n");
        printf("2. View Articles\n");
        printf("3. Search Articles\n");
        printf("4. Save Article to Favorites\n");
        printf("5. View Favorite Articles\n");
        printf("6. View File Content\n");
        printf("7. Save Articles to File\n");
        printf("8. Exit\n");
        printf("Enter the %snumber corresponding to your choice%s: ", COLOR_CYAN, COLOR_RESET);
        scanf("%d", &choice);
        getchar();

        switch (choice) {
            case 1: addArticle(); break;
            case 2: viewArticles(); break;
            case 3: searchArticle(); break;
            case 4: favoriteArticle(); break;
            case 5: viewFavorites(); break;
            case 6: viewFile(); break;
            case 7: manualSave(); break;
            case 8:
                printf("\e[1;1H\e[2J");
                printf("Exiting...\n");
                break;
            default:
                printf("Invalid choice. Please try again.\n");
                printf("Press" COLOR_YELLOW " Enter" COLOR_RESET " to continue");
                getchar();
        }
    } while (choice != 8);

    return 0;
}
