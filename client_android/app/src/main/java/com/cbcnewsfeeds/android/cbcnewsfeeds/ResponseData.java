package com.cbcnewsfeeds.android.cbcnewsfeeds;

import org.simpleframework.xml.Element;
import org.simpleframework.xml.ElementList;
import org.simpleframework.xml.Root;

import java.util.List;


@Root
public class ResponseData {

	@ElementList(inline = true, required = false)
	List<ItemGroup> itemGroup;
	public List<ItemGroup> getItemGroup() {
		return itemGroup;
	}


}
