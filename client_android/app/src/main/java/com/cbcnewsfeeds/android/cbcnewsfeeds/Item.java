package com.cbcnewsfeeds.android.cbcnewsfeeds;

import org.simpleframework.xml.Element;
import org.simpleframework.xml.Root;



@Root
public class Item {
	@Element
	String pubTime;
	@Element(required=false)
	String author;
	@Element
	String url;
	@Element
	String heading;
	@Element
	String description;
	@Element
	String img;
	public String getPubTime() {
		return pubTime;
	}
	public String getAuthor() {
		return author;
	}
	public String getUrl() {
		return url;
	}
	public String getHeading() {
		return heading;
	}
	public String getDescription() {
		return description;
	}
	public String getImg() {
		return img;
	}
}
