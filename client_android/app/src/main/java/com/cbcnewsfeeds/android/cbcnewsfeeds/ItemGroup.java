package com.cbcnewsfeeds.android.cbcnewsfeeds;

import org.simpleframework.xml.Element;
import org.simpleframework.xml.ElementList;
import org.simpleframework.xml.Root;

import java.util.List;


@Root
public class ItemGroup {
	@Element
	String category;
	@ElementList(inline = true, required = false)
	 List<Item> item;

	public String getCategory() {
		return category;
	}

	public List<Item> getItem() {
		return item;
	}


	 
}
