package com.cbcnewsfeeds.android.cbcnewsfeeds;

import android.content.Context;
import android.graphics.drawable.Drawable;
import android.support.v7.widget.RecyclerView;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ImageView;
import android.widget.TextView;

import java.io.InputStream;
import java.net.URL;

/**
 * Created by Ashok Sen Gurudayal on 15-03-2017.
 */

public class NewsAdapter extends RecyclerView.Adapter<NewsAdapter.NewsAdapterViewHolder>  {

    final private NewsItemClickListener mOnClickListener;
    private String newsHeadingDetails[];
    private String newsSummaryDetails[];
    public NewsAdapter(NewsItemClickListener listener){
        this.newsHeadingDetails = new String[]{"heading1","heading2","heading3","heading4","heading5"};
        this.newsSummaryDetails = new String[]{"summary1","summary2","summary3","summary4","summary5"};
        mOnClickListener = listener;
    }
    public class NewsAdapterViewHolder extends RecyclerView.ViewHolder implements View.OnClickListener {
        TextView newsHeading,newsSummary;
        ImageView newsImage;
        public NewsAdapterViewHolder(View view) {
            super(view);
            newsHeading = (TextView) view.findViewById(R.id.news_heading);
            newsSummary = (TextView) view.findViewById(R.id.news_summary);
            newsImage = (ImageView) view.findViewById(R.id.news_image);
            view.setOnClickListener(this);
        }

        @Override
        public void onClick(View v) {
            int clickedPosition = getAdapterPosition();
            mOnClickListener.onListItemClick(clickedPosition);

        }
    }
    @Override
    public NewsAdapter.NewsAdapterViewHolder onCreateViewHolder(ViewGroup viewGroup, int viewType) {
        Context context = viewGroup.getContext();
        int layoutIdForListItem = R.layout.newsitems;
        LayoutInflater inflater = LayoutInflater.from(context);
        boolean shouldAttachToParentImmediately = false;

        View view = inflater.inflate(layoutIdForListItem, viewGroup, shouldAttachToParentImmediately);
        return new NewsAdapterViewHolder(view);
    }

    @Override
    public void onBindViewHolder(NewsAdapter.NewsAdapterViewHolder holder, int position) {
        holder.newsHeading.setText(MainActivity.newsStructureList.get(position).getNewsHeading());
        holder.newsSummary.setText(MainActivity.newsStructureList.get(position).getNewsDescription());
        Log.d("newsimage","Start");
        holder.newsImage.setImageDrawable(MainActivity.newsStructureList.get(position).getImage());
        Log.d("newsimage","Done");
    }

    @Override
    public int getItemCount() {
        return MainActivity.newsStructureList.size();
    }

    public interface NewsItemClickListener {
        void onListItemClick(int clickedItemIndex);
    }

}
