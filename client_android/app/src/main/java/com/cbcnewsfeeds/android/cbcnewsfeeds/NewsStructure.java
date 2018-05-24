package com.cbcnewsfeeds.android.cbcnewsfeeds;

import android.graphics.drawable.Drawable;

/**
 * Created by Ashok Sen Gurudayal on 19-03-2017.
 */

public class NewsStructure {
    public void setNewsDescription(String newsDescription) {
        this.newsDescription = newsDescription;
    }

    public void setNewsHeading(String newsHeading) {
        this.newsHeading = newsHeading;
    }

    public String getNewsHeading() {

        return newsHeading;
    }

    public String getNewsDescription() {
        return newsDescription;
    }
    private String pubTime;
    private String author;
    private String url;
    private  String newsHeading;
    private  String newsDescription;
    private Drawable image;
    private String category;

    public String getCategory() {
        return category;
    }

    public void setCategory(String category) {
        this.category = category;
    }

    public String getPubTime() {
        return pubTime;
    }

    public void setPubTime(String pubTime) {
        this.pubTime = pubTime;
    }

    public String getAuthor() {
        return author;
    }

    public void setAuthor(String author) {
        this.author = author;
    }

    public String getUrl() {
        return url;
    }

    public Drawable getImage() {
        return image;
    }

    public void setImage(Drawable image) {
        this.image = image;
    }

    public void setUrl(String url) {
        this.url = url;
    }


}
